/*
 * ========================= map_mask.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.26
 *                                        修改 -- 2018.11.26
 * ----------------------------------------------------------
 *    制作 地图 mask 
 *  --------------------
 *  0 == 白点，不可行走区
 *  1 == 黑点，可行走区
 * ----------------------------
 */
#include "map_builder.h" 

//-------------------- C --------------------//
#include <string.h> //- memcpy
#include <cassert>

//-------------------- CPP --------------------//
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;
using std::vector;

namespace MapBuilder {//------- namespace : MapBuilder ---------------



//------------------- 提供给外部的函数 --------------------//
void mask_build( unsigned char *_data, int _w, int _h );
void merge_pix( int _color, std::vector<u8> &_map );

//-- 内部 func ---
void poisson_distr();

void independent_erase();
void lonely_point_erase( int _w, int _h );
void test_lonely_point_erase();
void odd_matrix_erase( int _w, int _h, int _step );



/* ===========================================================
 *                       mask_build
 * -----------------------------------------------------------
 * -- 改写 png 图片中的数据
 */
void mask_build(){

    //--------------------------//
    //  零，测试用，手动分配像素点
    //--------------------------//
    if( is_open_test == true ){
        test_lonely_point_erase();
    }

    //--------------------------//
    //  一，为每个像素随机分配 0/1
    //--------------------------//
    if( is_open_uniform_distr == true ){
        for_each(
            []( int w, int h ){
                //-- 靠边的一圈像素，统统设置为 1
                if( (h==0) || (h==fullWH.y-1) || (w==0) || (w==fullWH.x-1) ){
                    picMap_mask[h*fullWH.x + w] = 1;
                }else{
                    if( udi(dre) > N_PERCENT ){
                        picMap_mask[h*fullWH.x + w] = 1;
                    }else{
                        picMap_mask[h*fullWH.x + w] = 0;
                    }
                }
            }
        );
    }

    //--------------------------//
    //    泊松分布 黑白点
    //--------------------------//
    if( is_open_poisson_distr == true ){
        poisson_distr();     
    }

    //---------------------------------------//
    //  二，       合并 黑点 
    //---------------------------------------//
    if( is_open_merge_black == true ){
        for( int i=0; i<N_MERGE_BLACK; i++ ){
            merge_pix( 1, picMap_mask );
        }
    }

    //---------------------------------------//
    //     删除 孤立的 黑点  [优化] 
    //---------------------------------------//
    if( is_open_independent_erase ){
        independent_erase();
    }

    //--- end ---
}


/* ===========================================================
 *                  poisson_distr
 * -----------------------------------------------------------
 * -- 设置 泊松分部 
 */
void poisson_distr(){

    int w; //- tmp
    int h; //- tmp

    std::uniform_int_distribution<int> di_w(  5, fullWH.x-5  );
    std::uniform_int_distribution<int> di_h(  5, fullWH.y-5  );

    //-- 生成 5个 黑色 随机点 --
    black_poisson_pts.clear();
    for( int i=0; i<N_POISSON_BLACK; i++ ){
        black_poisson_pts.push_back( PixVec2{ di_w(dre), di_h(dre) } );
    }

    for( const auto& p : black_poisson_pts ){
        //-- 针对每个点，做柏松分布 --
        std::poisson_distribution<int> poisdi_w( p.x );
        std::poisson_distribution<int> poisdi_h( p.y );

        int times = udi_pois(dre);
        for( int i=0; i<times; i++ ){

            w = poisdi_w(dre);
            h = poisdi_h(dre);

            if( (w>0) && (w<fullWH.x-1) && (h>0) && (h<fullWH.y-1)  ){
                picMap_mask[ h*fullWH.x + w ] = 1; //- 黑点
            }
        }
    }
    
   //-- 生成 5个 白色 随机点 --
   white_poisson_pts.clear();
    for( int i=0; i<N_POISSON_WHITE; i++ ){
        white_poisson_pts.push_back( PixVec2{ di_w(dre), di_h(dre) } );
    }

    for( const auto& p : white_poisson_pts ){
        //-- 针对每个点，做柏松分布 --
        std::poisson_distribution<int> poisdi_w( p.x );
        std::poisson_distribution<int> poisdi_h( p.y );

        int times = udi_pois(dre);
        for( int i=0; i<times; i++ ){

            w = poisdi_w(dre);
            h = poisdi_h(dre);

            if( (w>0) && (w<fullWH.x-1) && (h>0) && (h<fullWH.y-1)  ){
                picMap_mask[ h*fullWH.x + w ] = 0; //- 白点
            }
        }
    }

}


/* ===========================================================
 *                 independent_erase
 * -----------------------------------------------------------
 * -- 删除 孤立的 黑白点
 */
void independent_erase(){

    for( int h=0; h<fullWH.y; h++ ){
        for( int w=0; w<fullWH.x; w++ ){
            lonely_point_erase( w, h );
        }
    }

}

/* ===========================================================
 *                 lonely_point_erase
 * -----------------------------------------------------------
 * -- 消除 孤立的 黑白点。只消除一个 
 * -- 当一个点的上下左右都 为异色，这个点就被同化
 */
void lonely_point_erase( int _w, int _h ){

    bool is_black; //- 当前处理的点是 黑点还是白点
    int same; //- 同色
    int anti; //- 异色
    if( picMap_mask[_h*fullWH.x + _w] == 1 ){
        is_black = true;
        same = 1;
        anti = 0;
    }else{
        is_black = false;
        same = 0;
        anti = 1;
    }

    //-- 检查左边 --
    if( (_w>0) && (picMap_mask[_h*fullWH.x + _w-1] == same) ){
        return;
    }
    //-- 检查右边 --
    if( (_w<fullWH.x-1) && (picMap_mask[_h*fullWH.x + _w+1] == same) ){
        return;
    }
    //-- 检查下边 --
    if( (_h>0) && (picMap_mask[(_h-1)*fullWH.x + _w] == same) ){
        return;
    }
    //-- 检查上边 --
    if( (_h<fullWH.y-1) && (picMap_mask[(_h+1)*fullWH.x + _w] == same) ){
        return;
    }

    //-- 确定 目标像素被孤立，将其同化 --
    picMap_mask[_h*fullWH.x + _w] = anti;
}

/* ===========================================================
 *                 lonely_point_erase
 * -----------------------------------------------------------
 * -- 针对上一函数的测试
 */
void test_lonely_point_erase(){

    //-- 重刷 整体色 --
    for_each(
        []( int w, int h ){
            picMap_mask[ h*fullWH.x + w ] = 1;
        }
    );

    //-- 像素点容器
    vector<PixVec2> points; 
    //-- 手动分配
    points.push_back( PixVec2{ 50, 50 } );
    points.push_back( PixVec2{ 51, 51 } );
        
    points.push_back( PixVec2{ 0, 50 } );
    points.push_back( PixVec2{ fullWH.x-1, 50 } );
    points.push_back( PixVec2{ 50, 0 } );
    points.push_back( PixVec2{ 50, fullWH.y-1 } );

    //--
    for( auto p : points ){
        picMap_mask[ p.y*fullWH.x + p.x ] = 0;
    }
}



/* ===========================================================
 *                 odd_matrix_erase
 * -----------------------------------------------------------
 * -- 奇数矩阵 清理器（清除孤立的 黑白点）
 * -- 以 7边长宫格为例
 * -- ***  存在一些 bug,此函数 暂时不使用  ***
 */
void odd_matrix_erase( int _w, int _h, int _step ){

    int halfStep = (_step-1)/2; //- n宫格 边长的一半

    //-- 太靠边的 点 不处理 --
    if( (_w-halfStep<0) || (_w+halfStep>fullWH.x) || 
        (_h-halfStep<0) || (_h+halfStep>fullWH.y) ){
        return;
    }

    bool is_black; //- 当前处理的点是 黑点还是白点
    if( picMap_mask[_h*fullWH.x + _w] == 1 ){
        is_black = true;
    }else{
        is_black = false;
    }

    //-- 遍历 7宫格的边缘一圈 --
    //-- 注意，不需要查看 7宫格的 4个角点。（这些点就算是 相反色点，也不影响 宫格内部图案的独立）
    int target; 
    if( is_black==true ){
        target = 1; //- 黑点找黑点
    }else{
        target = 0; //- 白点找白点
    }
    //-- 遍历 底边 --
    for( int w=(_w-halfStep+1); w<(_w+halfStep); w++ ){
        if( picMap_mask[(_h-halfStep)*fullWH.x + w] == target ){
            //-- 递归的最小规模是 3边长宫格 (九宫格)
            if( _step == 3 ){
                return;
            }else{
                odd_matrix_erase( _w, _h, _step-2 );
                return;
            }
        }
    }

    //-- 遍历 顶边 --
    for( int w=(_w-halfStep+1); w<(_w+halfStep); w++ ){
        if( picMap_mask[(_h+halfStep)*fullWH.x + w] == target ){
            //-- 递归的最小规模是 3边长宫格 (九宫格)
            if( _step == 3 ){
                return;
            }else{
                odd_matrix_erase( _w, _h, _step-2 );
                return;
            }
        }
    }

    //-- 遍历 左边 --
    for( int h=(_h-halfStep+1); h<(_h+halfStep); h++ ){
        if( picMap_mask[h*fullWH.x + (_w-halfStep)] == target ){
            //-- 递归的最小规模是 3边长宫格 (九宫格)
            if( _step == 3 ){
                return;
            }else{
                odd_matrix_erase( _w, _h, _step-2 );
                return;
            }
        }
    }

    //-- 遍历 右边 --
    for( int h=(_h-halfStep+1); h<(_h+halfStep); h++ ){
        if( picMap_mask[h*fullWH.x + (_w+halfStep)] == target ){
            //-- 递归的最小规模是 3边长宫格 (九宫格)
            if( _step == 3 ){
                return;
            }else{
                odd_matrix_erase( _w, _h, _step-2 );
                return;
            }
        }
    }

    //--- 现在，7宫格内部 确实是 孤立的 --
    //-- 将 7宫格内的 5宫格区域 的黑点，全写为 白点
    int anti_target;
    if( is_black==true ){
        target = 1;      //- 黑点找黑点
        anti_target = 0; //- 黑点涂白色
    }else{
        target = 0;      //- 白点涂黑色
        anti_target = 1; //- 白点涂黑色
    }
    for( int h=(_h-halfStep+1); h<(_h+halfStep); h++ ){
        for( int w=(_w-halfStep+1); w<(_w+halfStep); w++ ){

            if( picMap_mask[h*fullWH.x + w] == target ){
                picMap_mask[h*fullWH.x + w] = anti_target;
            }
        }
    }
}

/* ===========================================================
 *                      merge_pix
 * -----------------------------------------------------------
 * -- 通用
 * -- 对一张 随机像素图，合并其 1值 区域。
 * -- 根据 九宫格策略，如果一个像素的周围 8格中，有4格为1，则设置此像素为 1
 * -- param: _w
 * -- param: _h
 * -- param: _color -- 要针对那种颜色，进行 merge，通常为 1（black）
 * -- param: _map  -- 要改写哪张 map
 */
void merge_pix( int _color, std::vector<u8> &_map ){

    //---- 重排 map 全体单位的序号 顺序 --------
    //   速度会慢些，但可以接受
    dre.discard(7);
    std::shuffle( randWH.begin(), randWH.end(), dre );

    //--- 遍历 每个 map单位 ---
    int count; //-- 统计 单个像素，四周8格的 1 的个数
    for( const auto &p : randWH ){
        
        count = 0;
        //-- 遍历 9宫格 --
        for( int h=(p.y-1); h<=(p.y+1); h++ ){
            for( int w=(p.x-1); w<=(p.x+1); w++ ){
                //- 只处理周围 8 格 --
                if( !((w==p.x)&&(h==p.y)) ){
                    //- 防止 边界外访问
                    if( (w>=0) && (w<fullWH.x) && (h>=0) && (h<fullWH.y) ){
                        //- 只记录 要求色
                        if( _map[h*fullWH.x + w] == _color ){
                            count++;
                        }
                    }
                }
            }
        }
        //-- 改写 目标格的颜色
        if( count >= 4 ){
            _map[p.y*fullWH.x + p.x] = _color;
        }else{
            _map[p.y*fullWH.x + p.x] = 0; //- 不严谨
        }
    }
}





}//----------------- namespace : MapBuilder end -------------------

