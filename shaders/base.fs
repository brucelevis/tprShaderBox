#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

//-- 从 顶点着色器 传来的 数据（名称和类型 都要相同）
in vec2 TexCoord;

//-- sampler 系列 是 采样器数据类型。
//-- 现在，我们创建了 2个 纹理采样器
uniform sampler2D texture1;


in vec2 u_resolution;  //-- 矩形每个像素的 坐标 [-1,1]
uniform vec2  u_mouse; //-- 鼠标位置（在屏幕上哪个像素）(左上坐标系) [0,1]
uniform float u_time;  //-- glfwGetTime(); 未做缩放


//------------- 常量 --------------
#define PI 3.14159265359

//-------- color --------
vec3 color;
vec3 white   = vec3( 1.0, 1.0, 1.0 );
vec3 black   = vec3( 0.0, 0.0, 0.0 );
vec3 red     = vec3( 1.0, 0.0, 0.0 );
vec3 green   = vec3( 0.0, 1.0, 0.0 );
vec3 blue    = vec3( 0.0, 0.0, 1.0 );


//--------- lb [left_bottom] ------
//-- 左下 坐标系 [0,1]
vec2 lb;

//--------- lt [left_top] ------
//-- 左上 坐标系 [0,1]
//-- 和 鼠标同坐标系
vec2 lt;




//------------ 函数 -----------
//-- qualifer/限定符：
// in     -- 只读
// out    -- 只写
// inout  -- 可读可写
void prepare();


/* ====================================================
 *                      main
 * ----------------------------------------------------
 * -- 无视 texture1 的影响，一个纯粹的 fs
 */
void main()
{

    prepare();

    


    //- 分为 20 份
    vec2 f = floor( lt * 10.0 ) / 10.0;
    
    
    

    
    color = vec3( f.xxx );
    //color = vec3( v, v, 0.5 );
    //color = vec3( u_mouse, 0.5 );
    FragColor = vec4( color, 1.0 );
}



/* ====================================================
 *                     prepare
 * ----------------------------------------------------
 * -- 准备工作
 */
void prepare(){
    //--------------------------//
    //    初始化 lb, lt
    //--------------------------//
    //-- 左下坐标系 [0,1]
    lb.x = (u_resolution.x + 1.0)/2.0; //-- [0,1]
    lb.y = (u_resolution.y + 1.0)/2.0; //-- [0,1]

    //-- 左上 坐标系 [0,1]
    lt.x = (u_resolution.x + 1.0)/2.0; //-- [0,1]
    lt.y = (1.0 - u_resolution.y)/2.0; //-- [0,1]




}


