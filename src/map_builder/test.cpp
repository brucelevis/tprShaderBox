/*
 * ========================= test.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.02
 *                                        修改 -- 2018.12.02
 * ----------------------------------------------------------
 *    在 map 中间，绘制一个 标示实际窗口尺寸的 线框
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

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- SELF --------------------//
#include "random.h"
#include "PerlinNoise1D.h" 

#include "PerlinNoise3D.h" //- out 
 
using std::string;
using std::cout;
using std::endl;
using std::vector;


namespace MapBuilder {//------- namespace : MapBuilder ---------------


void test_hight();

static u32 seed = 0; //-- self_rand 函数使用的 种子


/* ===========================================================
 *                      test
 * -----------------------------------------------------------
 * -- 测试，绘制图形 试验。
 */
void test(){


    test_hight();
    return;
    //---------
    /*
    vector<PixVec2> ps; //- 所有要绘制的 像素点

    PixVec2 begPos { 10, fullWH.y/2 }; //- 曲线起始点

    int x;
    int y;


    PerlinNoise1DNext pnn( 0.06f, 50.0f );
    pnn.init();

    float sx = 0.0f; //- 一个 不规则地递增的值，用它来获取 pn


    for( int i=0; i<1000; i++ ){

        x = i;

        y = (int)pnn.next();

        //----
        x += begPos.x;
        y += begPos.y;

        ps.push_back( PixVec2{ x, y } );
    }

    //--------
    for( const auto &p : ps ){

        if( (p.x>=0) && (p.x<fullWH.x) && (p.y>=0) && (p.y<fullWH.y) ){
            picMap_mask[p.y*fullWH.x + p.x] = 4; //- blue
        }
    }
    */

}


/* ===========================================================
 *                    test_hight
 * -----------------------------------------------------------
 * -- 绘制 地形高度
 */
void test_hight(){


    PerlinNoise3D pn;
    pn.init();

    u8    vv;

    u32  minvv = 255; //- vv 最小值
    u32  bigvv = 0;   //- vv 中最大值

    float _w;
    float _h;
    float s;

    float mins = 256.0f; //- s 最小值
    float bigs = 0.0f; //- s 中最大值
    

    PngPix *pix_ptr; //- 指向每个像素 data 的指针
    
    for( int h=0; h<fullWH.y; h++ ){
        for( int w=0; w<fullWH.x; w++ ){
            
            pix_ptr =  pngData + (h*fullWH.x + w);

            //-- 跳过 深渊 部分 --
            if( picMap_mask[h*fullWH.x + w] == 0 ){
                pix_ptr->r = 45;
                pix_ptr->g = 45;
                pix_ptr->b = 45;
                pix_ptr->a = 255;
                continue;
            }
            

            //-- [0.0, 1.0]
            _w = (float)w/(float)fullWH.x;
            _h = (float)h/(float)fullWH.y;

            //-- 获得的 perlin值 在 [0.0, 1.0] 
            float times = 6.0f; 
            s = pn.noise( _w*times, _h*times, 0.8f );

                if( s > bigs ){
                    bigs = s;
                }
                if( s < mins ){
                    mins = s;
                }
            
            
            s *= 256;

            vv = (u8)s;

                if( vv > bigvv ){
                    bigvv = vv;
                }
                if( vv < minvv ){
                    minvv = vv;
                }
                
            vv /= 16;
            vv *= 16;

            
            pix_ptr->r = vv;
            pix_ptr->g = vv;
            pix_ptr->b = vv;
            pix_ptr->a = 255;



        }
    }
    
    cout << "\nmins = " << mins 
        << "\nbigs = " << bigs 
        << endl;

    cout << "\nminvv = " << (int)minvv
        << "\nbigvv = " << (int)bigvv 
        << endl;

    window();

    for_each(
        []( int w, int h ){

            switch( picMap_mask[h*fullWH.x + w] ){
                //case 0: write_pix( w, h, white ); break;
                //case 1: write_pix( w, h, black ); break;
                //case 2: write_pix( w, h, red ); break;
                //case 3: write_pix( w, h, green ); break;
                case 4: write_pix( w, h, blue ); break;
                default:
                    break;
                    //assert(0);
            }
        }
    );

}






}//----------------- namespace : MapBuilder end -------------------

