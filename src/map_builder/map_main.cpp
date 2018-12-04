/*
 * ========================= map_main.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.01
 *                                        修改 -- 2018.12.01
 * ----------------------------------------------------------
 *    map_builder 模块 主文件 
 * ----------------------------
 */
#include "map_builder.h"

//-------------------- C --------------------//
#include <cassert>

namespace MapBuilder {//------- namespace : MapBuilder ---------------


/* ===========================================================
 *                  map_builder
 * -----------------------------------------------------------
 * -- map builder 模块 主函数 
 */
void map_builder( unsigned char *_data, int _w, int _h ){

    //--------------------------//
    //  准备好 随机数引擎
    //--------------------------//
    dre = get_dRandEng();
    
    //--------------------------//
    //  根据参数，初始化 本地值
    //--------------------------//
    fullWH.x = _w;
    fullWH.y = _h;
    
    pngData = (PngPix*)_data;
    picMap_mask.resize( fullWH.x * fullWH.y );
    picMap_road.resize( fullWH.x * fullWH.y );

    mapMidPos = glm::vec2{ (float)(fullWH.x/2), (float)(fullWH.y/2) };

    //--------------------------//
    //   初始化  randWH
    //--------------------------//
    randWH.reserve( fullWH.x * fullWH.y );
    for_each(
        []( int w, int h ){
            randWH.push_back(  PixVec2{ w, h } );
        }
    );

    //--------------------------//
    //      制作 map mask
    //--------------------------//
    mask_build();

    //--------------------------//
    //      制作 road
    //--------------------------//
    if( is_open_road_build == true ){
        //road_build();
    }


    //--------------------------//
    //    绘制 游戏窗口线框
    //--------------------------//
    //window();

    //--------------------------//
    //      曲线 test
    //--------------------------//
    test();

    //--------------------------//
    //     渲染 map
    //--------------------------//
    //render_map();

    //--- end ---
}


/* ===========================================================
 *              for_each
 * -----------------------------------------------------------
 * -- 遍历整张 pic，按照常规规则来遍历
 */
void for_each( each_pix_FP _func ){

    for( int h=0; h<fullWH.y; h++ ){
        for( int w=0; w<fullWH.x; w++ ){
            _func( w, h );
        }
    }
}

/* ===========================================================
 *               for_each_with_randWH
 * -----------------------------------------------------------
 * -- 遍历整张 pic，但在每次遍历前，都对 像素序号 来一次乱序
 */
void for_each_with_randWH( each_pix_FP _func ){

    //-- 重排 w/h 序号 序列
    //   每次都要把 全体像素的 序号重排，
    //   速度会慢些，但可以接受
    dre.discard(7);
    std::shuffle( randWH.begin(), randWH.end(), dre );

    for( auto wh : randWH ){
        _func( wh.x, wh.y );
    }
}

/* ===========================================================
 *                     write_pix
 * -----------------------------------------------------------
 * -- 给一个像素单位 写入 rbga 信息
 */
void write_pix( int _w, int _h, const PngPix &_pixData ){

    PngPix *pix_ptr; //- 指向每个像素 data 的指针
    pix_ptr =  pngData + (_h*fullWH.x + _w);

    pix_ptr->r = _pixData.r;
    pix_ptr->g = _pixData.g;
    pix_ptr->b = _pixData.b;
    pix_ptr->a = _pixData.a;
}






}//----------------- namespace : MapBuilder end -------------------

