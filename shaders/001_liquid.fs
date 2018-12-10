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
vec2 random2( vec2 p );


/* ====================================================
 *                      main
 * ----------------------------------------------------
 * -- 无视 texture1 的影响，一个纯粹的 fs
 */
void main(){

    prepare();

    //-- 
    lb *= 1.0;

    // Tile the space
    vec2 i_st = floor(lb);
    vec2 f_st = fract(lb);

    float m_dist = 1.;  // minimun distance

    //- 遍历 3*3矩阵 的 每个点 --
    for (int j= -1; j <= 1; j++ ) {
        for (int i= -1; i <= 1; i++ ) {

            //- 3*3矩阵 的 每个点，距离 本像素的 坐标
            //- [-1.0, -1.0] -> [1.0, 1.0] 
            vec2 neighbor = vec2(float(i),float(j));

            // Random position from current + neighbor place in the grid
            vec2 offset = random2(i_st + neighbor);
            //vec2 offset = i_st + neighbor;

            // Animate the offset
            offset = 0.5 + 0.5*sin(u_time + 6.2831*offset);

            // Position of the cell
            vec2 pos = neighbor + offset - f_st;

            // Cell distance
            float dist = length(pos);

            // Metaball it!
            m_dist = min(m_dist, m_dist*dist);
        }
    }

    // Draw cells
    color += step(0.092, m_dist);

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


/* ====================================================
 *                    random2
 * ----------------------------------------------------
 * -- 
 */
vec2 random2( vec2 p ) {

    //-- 计算 参数向量 p 于两个 向量的 点积。
    //-  就是 夹角的 cos值  [0.0, 1.0]
    float dot_1 = dot(p,vec2(127.1,311.7));
    float dot_2 = dot(p,vec2(269.5,183.3));


    vec2 r = sin( vec2( dot_1, dot_2 ) );
    r *= 43758.5453;

    return fract( r );

}






