#version 330 core

//-- 片段着色器的 主输出：颜色
out vec4 FragColor;

in vec2 u_resolution;  //-- 矩形每个像素的 坐标 [-1,1]
uniform vec2  u_mouse; //-- 鼠标位置（在屏幕上哪个像素）(左上坐标系) [0,1]
uniform float u_time;  //-- glfwGetTime(); 未做缩放



//------------- 常量 --------------
#define PI 3.14159265359

vec3 color; //-- 主色
vec3 white   = vec3( 1.0, 1.0, 1.0 );
vec3 black   = vec3( 0.0, 0.0, 0.0 );
vec3 red     = vec3( 1.0, 0.0, 0.0 );
vec3 green   = vec3( 0.0, 1.0, 0.0 );
vec3 blue    = vec3( 0.0, 0.0, 1.0 );

//--------- st ------
//-- 左下 坐标系 [0,1]
vec2 st;

//-- 左上 坐标系 [0,1]
//-- 和 鼠标同坐标系
vec2 lt;

//-- 极坐标。画面中心为 0点. [-0.5, 0.5]
vec2 pos;



//------------ 函数 -----------
//-- qualifer/限定符：
// in     -- 只读
// out    -- 只写
// inout  -- 可读可写
float func_square( in float x ); //-- 真正的运算函数
float func_2( in float x );
float func_3( in float x );


//-- 参数 len 是 当前坐标-鼠标坐标的 距离向量
//-- 返回 当前坐标 pct
float mouse_boll( in vec2 len );

//----------------------------

float plot( in float x, in float y );


//-- 绘制一个 矩形。
float box( in vec2 _st, in vec2 _size );


float random( vec2 _st );



/* ====================================================
 *                      main
 * ----------------------------------------------------
 * -- fs程序中，此 main 函数，会对 每一个像素都运行一遍。
 */
void main()
{

    //-- 目前 gl_FragCoord 失效，用此法来代替 --
    //-- 为 左下坐标系 [0,1]
    st.x = (u_resolution.x + 1.0)/2.0; //-- [0,1]
    st.y = (u_resolution.y + 1.0)/2.0; //-- [0,1]

    lt.x = (u_resolution.x + 1.0)/2.0; //-- [0,1]
    lt.y = (1.0 - u_resolution.y)/2.0; //-- [0,1]

    pos = vec2(0.5) - st; //-- 极坐标. [-0.5, 0.5]

        //----------------------

    /*
    color = white;

    color = mix( color, 
                blue, 
                plot( func_square(st.x), st.y )
                );
    
    color = mix( color, 
                red, 
                plot( step(0.5, st.x), st.y )
                );

    color = mix( color, 
                black, 
                mouse_boll( lt - u_mouse )
                );
    */

        //-------------------------------
    
    /*
    vec2 bl = step( vec2(0.1), st );
    float pct = bl.x * bl.y;

    vec2 tr = step( vec2(0.1), 1.0-st );
    pct *= tr.x * tr.y;
    

    color = vec3( pct );
    */

        //-------------------------

    /*
    color = red; //-- 此处没用


    float r = length( pos )*2.0; //-- 半径长度的2倍
    float a = atan( pos.y, pos.x ); 
                //-- 求的 当前像素相对 极坐标原点 的夹角。

    float f = sin( a*1.0 ); //-- n倍夹角的 cos值（x轴长度）

    //-- 要么 1.0， 要么 0.0 其边界就是我们的 形状。
    //-- 此处的边界就是 f。
    
    //color =  vec3( smoothstep(f-0.005,f, r)  - 
    //               smoothstep(f,f+0.005, r) );

    color = vec3( step( 1, a )
            );
    */

        //----------------

    /*
    float aa = box( st, vec2(0.45) );
    color = white;
    color = mix( color, red, aa );
    */

        //------------------
    /*
    float rnd = random( st );

    color = vec3(rnd);
    */

        //------------------

    //-- 像素坐标系，将画面长宽都分为 20 格。
    vec2 bitxy = floor( st * vec2(20.0) ); 

    bitxy = bitxy / vec2(20.0);




    color = vec3( 0.9, 0.2, 0.8 );

    FragColor = vec4( color, 1.0 );
   
}



//-- 获得 线平滑边界 强度
//-- 返回值为 本像素 与 目标线关联的 强度
float plot( in float x, in float y ){
  return  smoothstep( x-0.005, x, y ) -
          smoothstep( x, x+0.005, y );
}


//-- 绘制一个 矩形。
//-- _st   -- 提供给 step系函数，做 x 轴用
//-- _size -- 矩形的 宽长 直径。
float box( in vec2 _st, in vec2 _size ){

    _size = vec2(0.5) - _size*0.5;

    vec2 uv = step( _size, _st ) *
              step( _size, vec2(1.0) - _st );

    return uv.x * uv.y;

}


//-- 随机函数
float random( vec2 _st ){
    return fract(
                 sin(
                     dot(
                         st.xy,
                         vec2(12.9898,78.233)
                        )
                    ) * 43758.5453123 //* u_time
                );
}


//========================================================//

//-- y = x^2
float func_square( in float x ){
    return x * x;
}

//-- y = sin(10x)
float func_2( in float x ){
    return sin( 10 * x );

}

float func_3( in float x ){
    return -x;
}


//-- 参数 len 是 当前坐标-鼠标坐标的 距离向量
//-- 返回 当前坐标 pct
float mouse_boll( in vec2 len ){

    //float l = 0.03 - length( len );

    //-- 此处用 dot 代替 sqrt系列函数，提高运算速度
    float l = 0.002 - dot( len, len );

    if( l > 0 ){
        return 0.7;
    }else{
        return 0.0;
    }

}











