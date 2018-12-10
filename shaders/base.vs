#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


out vec2 TexCoord; //-- 输出给 fs 的颜色数据 坐标
out vec2 u_resolution; //-- 顶点的世界空间坐标

uniform mat4 transform; //-- 矩阵 变换 变量。

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position =  projection * view * model * vec4( aPos, 1.0 ); 

    TexCoord = vec2(aTexCoord.x, aTexCoord.y);

    //-- 将每个像素在 window 上的坐标，传递给 fs。 [-1,1] --
    u_resolution = vec2( gl_Position.x, gl_Position.y );
}

