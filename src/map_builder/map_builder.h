/*
 * ========================= map_builder.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.26
 *                                        修改 -- 2018.11.26
 * ----------------------------------------------------------
 *    制作 map 数据
 * ----------------------------
 */
#ifndef _TPR_MAP_BUILDER_H_
#define _TPR_MAP_BUILDER_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- SELF --------------------//
#include "random.h"
#include "PixVec.h"
#include "config.h" 


namespace MapBuilder {//------- namespace : MapBuilder ---------------

using each_pix_FP = void (*)( int, int);

//-- 1个 png 像素 的 data 
struct PngPix{
    u8 r {};
    u8 g {};
    u8 b {};
    u8 a {};
};

//================== 外部可调用函数 ==================
void map_builder( unsigned char *_data, int _w, int _h );

//================== 模块内通用函数 函数 ==================
void for_each( each_pix_FP _func );
void for_each_with_randWH( each_pix_FP _func );
void write_pix( int _w, int _h, const PngPix &_pixData );
//---

void mask_build();
void road_build();
void render_map();
void window();
void merge_pix( int _color, std::vector<u8> &_map );

void test();



//====================== 流程 开关 ===========================
//--- mask ----
inline bool is_open_test = false; //-- 是否开启 测试（手动分配像素点）
inline bool is_open_uniform_distr = true; //- 是否开启，均匀分部 0／1 流程
inline bool is_open_poisson_distr = true;  //- 是否开启 不均匀封闭，制造 带有疏密关系的 0/1 分布
inline bool is_open_merge_black = true; //-- 是否启动 merge_black 流程
inline bool is_open_independent_erase = true; //-- 删除 孤立的黑点 流程 [优化]

//--- road_build ----
inline bool is_open_road_build = true; //- 是否开启 距离场 流程



//====================== 变量 ===========================
//----- 地图生成器参数 ----
inline int N_PERCENT = 5755; //- [1,1000], 1 占了多大比例
inline int N_MERGE_BLACK = 10;  //- 合并 1像素 函数，被反复执行几次 

//-- 通过调整 黑白区 柏松分部的个数，可以让世界变得更整体，or更分裂。
//-- 随着向地图边缘探索，白泊松的个数 将在一个 更大的区间内浮动，进而变得更加不规则。
inline int N_POISSON_BLACK = PIX_TIMES * 10; //- 黑 泊松分部 个数（随地图增大而变多）
inline int N_POISSON_WHITE = PIX_TIMES * 8; //- 白 泊松分部 个数（随地图增大而变多）

//--------- MapBuilder 模块内部 通用变量 ------
inline PixVec2 fullWH; //-- 目标图长宽

inline PngPix white  { 160,160,160, 255 }; //- 0
inline PngPix black  { 50, 50, 50, 255 };  //- 1
inline PngPix red    { 130, 60, 60, 255 }; //- 2
inline PngPix green  { 50, 130, 50, 255 }; //- 3
inline PngPix blue   { 60, 60, 130, 255 }; //- 4 -- 距离场 边界线
inline PngPix yellow { 130, 120, 55, 255 }; //- 5
inline PngPix deep_blue { 45, 60, 65, 255 }; //- 6



inline PngPix *pngData {nullptr};

inline std::vector<PixVec2> black_poisson_pts; //- 黑色 泊松点 
inline std::vector<PixVec2> white_poisson_pts; //- 白色 泊松点

inline std::vector<u8> picMap_mask; //- 地图运算在这个矩阵上完成，最后根据这个矩阵，转换为颜色
inline std::vector<PixVec2> randWH; //-  一个乱序存储的，pic的横纵坐标 序号

inline std::vector<u8> picMap_road; //- 计算道路，使用的 地图矩阵

inline glm::vec2 mapMidPos; //- map 中心坐标

inline std::default_random_engine  dre; //-随机数引擎实例，需要借用一个 全局随机数引擎
inline std::uniform_int_distribution<int> udi(1,10000); //- 均匀分部器 [1,10000]
inline std::uniform_int_distribution<int> udi_pois(1000,3000); //- 均匀分部器 [1000,3000]



}//----------------- namespace : MapBuilder end -------------------
#endif
