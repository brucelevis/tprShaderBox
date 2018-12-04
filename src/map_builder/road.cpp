/*
 * ========================= road.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.01
 *                                        修改 -- 2018.12.01
 * ----------------------------------------------------------
 *    距离场 --> 制作 地图路径 
 * ----------------------------
 */
#include "map_builder.h"

//-------------------- C --------------------//
#include <cassert>
#include <stdlib.h> //- abs
#include <math.h>  

//-------------------- CPP --------------------//
#include <iostream>
#include <string>

//------------------- SELF --------------------//
#include "random.h"
#include "PerlinNoise1D.h" 

using std::string;
using std::cout;
using std::endl;
using std::vector;

namespace MapBuilder {//------- namespace : MapBuilder ---------------


//---- 局部函数／变量 -----

glm::vec2 get_rightDirec( glm::vec2 _direc );
float get_road_width( float _x );


vector<PixVec2> df_pts; //- 4个 距离场 核心点

/* ===========================================================
 *                  road_build
 * -----------------------------------------------------------
 * -- 
 */
void road_build(){

    //--------------------------//
    //  生成 4 个距离场 核心
    //  避免 某个点 出现在 画面中央，这会导致 路线疯狂旋转
    //--------------------------//
    std::uniform_int_distribution<int> di_w(  0, fullWH.x/2  );
    std::uniform_int_distribution<int> di_h(  0, fullWH.y/2  );

    //--- 左下坐标系 ---
    //-- 左下区 --
    df_pts.push_back( PixVec2{ di_w(dre) + 0,
                                di_h(dre) + 0 });
    //-- 右下区 --
    df_pts.push_back( PixVec2{ di_w(dre) + fullWH.x/2,
                                di_h(dre) + 0 });
    //-- 左上区 --
    df_pts.push_back( PixVec2{ di_w(dre) + 0,
                                di_h(dre) + fullWH.y/2 });
    //-- 右上区 --
    df_pts.push_back( PixVec2{ di_w(dre) + fullWH.x/2,
                                di_h(dre) + fullWH.y/2 });

    
    //----- 将 4 个 距离场核心点 画出来 tmp --
    for( auto p : df_pts ){
        picMap_mask[p.y*fullWH.x + p.x] = 2; //- red
    }

    //-----------------------------------------//
    //
    //       通过寻路机器人，绘制一条 道路
    //
    //-----------------------------------------//

    //--- 初始化 机器人 状态 ---
    glm::vec2 robBegPos; //- 机器人 起始坐标
    glm::vec2 robEndPos; //- 机器人 结束坐标
    glm::vec2 robPos;    //- 遍历用
    //-- 生成 机器人 起始坐标 --
    robBegPos = glm::vec2{ (float)(udi(dre)%fullWH.x),
                            (float)(udi(dre)%fullWH.y) };
    if( udi(dre) > 5000 ){
        if( udi(dre) > 5000 ){
            robBegPos.x = 0.0f;
        }else{
            robBegPos.x = (float)(fullWH.x-1);
        }
    }else{
        if( udi(dre) > 5000 ){
            robBegPos.y = 0.0f;
        }else{
            robBegPos.y = (float)(fullWH.y-1);
        }
    }
    robPos = robBegPos; 

    //----------------------------//
    glm::vec2 robDirec;      //-- 机器人方向向量（标准化），初始时，朝向 map中心
    glm::vec2 robRightDirec; //- 机器人右手方向向量(标准化)
    glm::vec2 targetDirec;   //- 目标方向
    glm::vec2 yaw;          //- 偏航向量（非标准化）机器人方向到目标方向的差（每走一步都要算
    //--------
    robDirec = glm::normalize( glm::vec2{ mapMidPos-robPos } );
        
    glm::vec2 roadPos;  //- 遍历路宽用的 坐标。初始时位于 机器人左手。
                        //-（配合 右手方向向量，和路宽，计算 路面单位坐标
    float road_left_off; //- 路左端 距 路中心 的距离，会动态变化
    float  road_width;     //- 路宽，会动态变化

    //-- 距离场核心集pos 的 glm::vec2 版 --
    vector<glm::vec2> targetsPos;
    //vector<glm::vec2> rtargetsPos; //- 逆序容器，
    for( auto p : df_pts ){
        targetsPos.push_back( glm::vec2{ (float)p.x, (float)p.y } );
    }
    //-- 乱序之 --
    std::shuffle( targetsPos.begin(), targetsPos.end(), dre );
    //- 迭代器，用来访问这个 乱序核心集 的每个元素
    auto target_pit = targetsPos.begin();
    //- 初始化 目标方向
    targetDirec = glm::normalize( *target_pit - robPos );

    //----------------------------//
    //- 负责 road_left_off 的变化 --
    PerlinNoise1DNext pnn_left_off( 0.3f, 5.0f );
    pnn_left_off.init();

    //- 负责 road_rad 的变化 --
    PerlinNoise1DNext pnn_rad( 0.06f, 15.0f );
    pnn_rad.init();


    int rw; //- tmp
    int rh; //- tmp
    //--------------------------------------//
    //           走一步，渲染一次
    //--------------------------------------//
    int steps = 0; //- 机器人行走步数
    while( steps < 10000 ){
        //--------------------------------//
        //  若 机器人方向 和 目标方向 接近平行
        //  更新 目标方向
        //--------------------------------//
        //-- 通过计算点积，来判断 两向量 是否平行 --
        if( glm::dot(robDirec, targetDirec) > 0.85f ){
            if( (target_pit+1) == targetsPos.end() ){
                //- 再一次乱序
                std::shuffle( targetsPos.begin(), targetsPos.end(), dre );
                //-- 将 targetsPos 逆序 --
                //rtargetsPos.assign( targetsPos.begin(), targetsPos.end() );
                //targetsPos.assign( rtargetsPos.rbegin(), rtargetsPos.rend() );

                target_pit = targetsPos.begin();
            }else{
                target_pit++;
                targetDirec = glm::normalize( *target_pit - robPos );
            }
        }

        //--------------------------------//
        //    更新 机器人方向 向量
        //--------------------------------//
        yaw = targetDirec - robDirec;
        yaw = yaw * 0.015f; 
        robDirec = glm::normalize( robDirec + yaw );

        //--------------------------------//
        //  沿着新方向，机器人走 1单位
        //--------------------------------//
        robPos += robDirec;

        //--------------------------------//
        //  如果机器人走出了地图超过 5 格，就 结束循环
        //--------------------------------//
        rw = (int)(robPos.x);
        rh = (int)(robPos.y);
        if( (rw==0) || (rw==fullWH.x-1) || 
            (rh==0) || (rh==fullWH.y-1) ){
            robEndPos = robPos; //- 记录 机器人结束坐标
        }else
        if( (rw<0-5) || (rw>=fullWH.x+5) || 
            (rh<0-5) || (rh>=fullWH.y+5) ){
            break;
        }

        //--------------------------------//
        //   
        //--------------------------------//
        road_left_off = 4.0f + pnn_left_off.next();

        //-- 计算出 机器人 右手方向 向量（标准化） --
        robRightDirec = get_rightDirec( robDirec );

        //-- 计算出 机器人 左手坐标 --
        roadPos = robPos - robRightDirec*road_left_off;

        //-- 给 路左边起点 涂上颜色 --
        rw = (int)(roadPos.x);
        rh = (int)(roadPos.y);
        if( (rw>=0) && (rw<fullWH.x) && (rh>=0) && (rh<fullWH.y) ){
            picMap_road[ rh*fullWH.x + rw] = 1; //- black
        }
        
        //--------------------------------//
        //   从 机器人左手坐标，沿着 右手方向，一次走1个单位，
        //   一直走满 2*road_rad 距离
        //   沿途踩到的所有 像素，都涂上颜色
        //--------------------------------//
        road_width = 6.0f + pnn_rad.next();
        for( int s=0; s<(int)road_width; s++  ){
            //-- 走一步
            roadPos += robRightDirec;
            //-- 给踩到的 路宽像素 涂上颜色
            rw = (int)(roadPos.x);
            rh = (int)(roadPos.y);
            if( (rw>=0) && (rw<fullWH.x) && (rh>=0) && (rh<fullWH.y) ){
                picMap_road[ rh*fullWH.x + rw] = 1; //- black
            }
        }
        //----
        steps++;
    }

    //---------------------------//
    //      合并 road 黑点
    //---------------------------//
    
    for( int i=0; i<4; i++ ){
        merge_pix( 1, picMap_road );
    }
    

    //--------------------------------------//
    //   以 机器人 起点／终点 为中心，将一个圆形区域像素 都写上
    //--------------------------------------//
    
    for( int h=0; h<fullWH.y; h++ ){
        for( int w=0; w<fullWH.x; w++ ){
            
            //-- 围绕起点 绘制圆 --
            rw = abs(w-(int)robBegPos.x);
            rh = abs(h-(int)robBegPos.y);
            if( (rw*rw + rh*rh) <= 36 ){
                picMap_road[ h*fullWH.x + w] = 1; //- black
                continue;
            }
            //-- 围绕终点 绘制圆 --
            rw = abs(w-(int)robEndPos.x);
            rh = abs(h-(int)robEndPos.y);
            if( (rw*rw + rh*rh) <= 36 ){
                picMap_road[ h*fullWH.x + w] = 1; //- black
            }
        }
    }
    
    


}

/* ===========================================================
 *                    get_rightDirec
 * -----------------------------------------------------------
 * -- 计算一个 v2 向量，在 二维坐标系中的 右手向量（垂直向量）(标准化)
 */
glm::vec2 get_rightDirec( glm::vec2 _direc ){

    glm::vec3 direc{ _direc.x, _direc.y, 0.0f };
    glm::vec3 zDirec{ 0.0f, 0.0f, 1.0f }; //- z轴向量

    glm::vec3 rh = glm::cross( direc, zDirec ); //-  叉乘

    return glm::normalize( glm::vec2{ rh.x, rh.y } );
}


/* ===========================================================
 *                   get_road_width
 * -----------------------------------------------------------
 * -- 一个随机 波形图，
 * -- 传入 x， 求得 y
 */
float get_road_width( float _x ){

    float y; //- return

    float freq = 1.0f; //- 频率
    float ampl = 5.0f; //- 振幅

    y = sin( _x * freq );

    y *= ampl;
    y += 1*ampl + 5; //- 始终确保 y>5 
    return y; //- tmp
}



}//----------------- namespace : MapBuilder end -------------------

