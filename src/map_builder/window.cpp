/*
 * ========================= window.cpp ==========================
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

using std::string;
using std::cout;
using std::endl;
using std::vector;

namespace MapBuilder {//------- namespace : MapBuilder ---------------


/* ===========================================================
 *                         window
 * -----------------------------------------------------------
 * -- 目标 窗口为 52*40 游戏像素
 */
void window(){

    //-- 窗口线框 长宽，和 半径 --
    int ww = 52;
    int wh = 40;
    int wrad_w = ww/2;
    int wrad_h = wh/2;

    vector<PixVec2> lineps; //- 线框的所有 像素点 

    //- 窗口 左下角坐标 --
    PixVec2 leftBottom{ (int)mapMidPos.x-wrad_w, (int)mapMidPos.y-wrad_h };

    //-- 下边，上边 --
    for( int w=leftBottom.x; w<(leftBottom.x+ww); w++ ){
        lineps.push_back( PixVec2{ w, leftBottom.y } );
        lineps.push_back( PixVec2{ w, leftBottom.y+wh } );
    }

    //-- 左边，右边 --
    for( int h=leftBottom.y; h<(leftBottom.y+wh); h++ ){
        lineps.push_back( PixVec2{ leftBottom.x, h } );
        lineps.push_back( PixVec2{ leftBottom.x+ww, h } );
    }

    //-----
    for( const auto &p : lineps ){
        picMap_mask[p.y*fullWH.x + p.x] = 4; //- blue
    }


}


}//----------------- namespace : MapBuilder end -------------------

