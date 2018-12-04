/*
 * ========================= render_map.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.26
 *                                        修改 -- 2018.11.26
 * ----------------------------------------------------------
 *    渲染 地图 到 窗口
 * ----------------------------
 */
#include "map_builder.h"

//-------------------- C --------------------//
#include <cassert>

namespace MapBuilder {//------- namespace : MapBuilder ---------------


/* ===========================================================
 *                  render_map
 * -----------------------------------------------------------
 * -- 
 */
void render_map(){

    //-- 先改写 mask 颜色信息 --
    //-  左下坐标系
    for_each(
        []( int w, int h ){

            switch( picMap_mask[h*fullWH.x + w] ){
                case 0: write_pix( w, h, white ); break;
                case 1: write_pix( w, h, black ); break;
                case 2: write_pix( w, h, red ); break;
                case 3: write_pix( w, h, green ); break;
                case 4: write_pix( w, h, blue ); break;
                default:
                    assert(0);
            }
        }
    );
    

    //-- 再改写 road 颜色信息 --
    for_each(
        []( int w, int h ){

            switch( picMap_road[h*fullWH.x + w] ){
                //case 0: write_pix( w, h, white ); break;
                case 1: write_pix( w, h, yellow ); break;
                //case 2: write_pix( w, h, red ); break;
                case 3: write_pix( w, h, green ); break;
                //case 4: write_pix( w, h, blue ); break;
                //default:
                    //assert(0);
            }
        }
    );
}


}//----------------- namespace : MapBuilder end -------------------

