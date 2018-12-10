/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 * 
 *    main();
 *    
 * ----------------------------
 */
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- C ----------------------//
#include <string.h> //- memset, memcpy
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>

//------------------- Libs --------------------//


//------------------- SELF --------------------//
#include "globfunc.h"
#include "global.h"

#include "glob_gl.h"
#include "RectVertics.h" //-- 矩形 顶点数据

#include "ShaderProgram.h" 
#include "Camera.h" 
#include "Texture.h" 
#include "Model.h"

#include "map_builder.h"

#include "config.h" 
 
using std::cout;
using std::endl;
using std::string;
using std::vector;


//------------------- 从外部获得的 函数 ----------------
extern void random_test();


//---------------------- 局部 变量 ---------------------
static double cursor_xpos;
static double cursor_ypos;
static glm::vec2 cursor_v2;


/* ===========================================================
 *                        main
 * -----------------------------------------------------------
 */
int main(){
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare();
    
    //==========================================//
    //                  TEST
    //------------------------------------------//
        //cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
        //return(0);
    //------------------------------------------//
    //                  glfw
    //------------------------------------------//
    glfw_init();
    glfw_hints_set();
    glfw_window_creat();
    glfw_oth_set();
    glfw_callback_set();

    //------------------------------------------//
    //                  glad
    //------------------------------------------//
    glad_init();
    glad_set();

    //---------------------------------------------//
    //                shader_program
    //---------------------------------------------//
    ShaderProgram rect_shader( "/shaders/base.vs",
                               "/shaders/001_liquid.fs" );
    rect_shader.init();

    //--- 
    rect_shader.get_uniform_location( "model" );
    rect_shader.get_uniform_location( "view" );
    rect_shader.get_uniform_location( "projection" );

    rect_shader.get_uniform_location( "texture1" );

    rect_shader.get_uniform_location( "u_mouse" ); //- 向 shader 传入鼠标信息
    rect_shader.get_uniform_location( "u_time" );  //- 向 shader 传入时间信息

    rect_shader.use_program();

    //---------------------------------------------//
    //          创建／初始化  所有 模型 
    //---------------------------------------------//
    Texture textel_1( "/textures/ground_160_120.png" );


    textel_1.set_png_builder( &MapBuilder::map_builder );

    
    //-- 生成一个 w * h 像素的 顶点数据集
    RectVertics rv_1( WORK_WIDTH, WORK_HEIGHT );

    Model canvas;
    canvas.set_VBO( rv_1.get_data(),
                    rv_1.get_size(),
                    rv_1.get_stride()  
                    );
    
    canvas.add_texture( textel_1 );
    
    canvas.set_shader_program( &rect_shader );

    canvas.init();

    canvas.set_translate( glm::vec3( -(WORK_WIDTH / 2.0f), -(WORK_HEIGHT / 2.0f), 0.0f ) );
    

    //---------------------------------------------//
    //                texture
    //---------------------------------------------//
    //-- 必须要激活 shaderProgram，这样才能修改其 uniform 变量值。
    rect_shader.use_program();
    //-- 为 两个 uniform 变量 texture1,texture2  设置值。
    //-- 分别指向 GL_TEXTURE0，GL_TEXTURE1 这两个 纹理单元。
    glUniform1i( rect_shader.uniform_location( "texture1" ), 0);


    //---------------------------------------------//
    //                 camera
    //---------------------------------------------//
    Camera camera; //-- 本游戏暂时只有 一个 摄像机
    camera.init();
    bind_camera_current( &camera ); //-- 将其绑定为 当前摄像机


    //---------------------------------------------//
    //           main render loop
    //---------------------------------------------//
    while( !glfwWindowShouldClose( window ) ){

        //------- 刷新 deltaTime ---------//
        update_time();

        //-------- u_time -----------//
        rect_shader.use_program();
        glUniform1f( rect_shader.uniform_location( "u_time" ),
                    (float)glfwGetTime() );

        //-------- u_mouse ----------//
        glfwGetCursorPos( window, &cursor_xpos, &cursor_ypos );

        cursor_v2 = glm::vec2( (float)cursor_xpos / (float)SCR_WIDTH,
                               (float)cursor_ypos / (float)SCR_HEIGHT
                               );

        rect_shader.use_program();
        glUniform2fv( rect_shader.uniform_location( "u_mouse" ),
                    1,
                    (const GLfloat*)(glm::value_ptr( cursor_v2 ))
                    );

        //------- input 处理 -----------//
        //-- 目前这个版本 非常简陋
		processInput( window );

        //--------- 重绘背景 ---------
        glClearColor( 0.25f, 0.24f, 0.75f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                    //-- 在每一帧的新绘制之前，清除上一帧的 颜色缓冲 和 深度缓冲


        //------------------------------------------//
        //                 
        //------------------------------------------//
        rect_shader.use_program();
        rect_shader.send_mat4_view_2_shader( 
                                camera_current()->update_mat4_view()
                                );

        rect_shader.send_mat4_projection_2_shader(
                                camera_current()->update_mat4_projection()
                                );

        //-------------------- 绘制图形 -----------------------

        canvas.model_draw();


        //-- check and call events and swap the buffers
		glfwPollEvents();   //-- 处理所有 处于 event queue 中的 待决 event。
		glfwSwapBuffers( window );

        //------------ 显示数据到终端 -----------//

            /*
            print_counter++;
            glm::vec3 pos;
            if( (print_counter%100) == 0 ){
                pos = camera_current()->get_cameraPos();
                cout << pos.x << ", "
                    << pos.y << ", "
                    << pos.z
                    << endl;
            }
            */

    }//---------- while render loop end -----------//


    //------------ 删除 所有 model -------------
    canvas.model_delete();

    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}







