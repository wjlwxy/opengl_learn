#shader Vertex
// 声明使用GLSL的版本为3.3核心规范。#version指令必须是shader程序的第一行，用于指定使用的GLSL版本。
#version 330 core

layout(location = 0) in vec4 position; // 定义输入变量position，类型为vec4（四维向量）。layout(location = 0)指定了这个输入变量的位置索引为0。在应用程序中，可以通过这个索引将数据从顶点缓冲区传递给顶点着色器。
// texCoord通过VertexArray::AddBuffer绑定顶点数组，从而将 纹理坐标 传给 texCoord
layout(location = 1) in vec2 texCoord; // 定义输入变量texCoord，类型为vec2（二维向量）。layout(location = 1)指定了这个输入变量的位置索引为1。这个变量通常用于存储纹理坐标，以便在片段着色器中对纹理进行采样。

out vec2 v_TexCoord; // 定义输出变量v_TexCoord，类型为vec2。out 关键字表示这个变量的值将从顶点着色器传递到片段着色器。这里传递的是纹理坐标，以便片段着色器可以使用这些坐标来确定从纹理图像中采样哪个位置的颜色。

uniform mat4 u_MVP; // uniform 表示从数据从cpu传过来

void main()
{
	gl_Position = u_MVP * position; // 将顶点的位置信息从输入变量position传递给OpenGL内部的gl_Position变量。gl_Position是一个特殊的输出变量，用于指定顶点在裁剪空间中的位置。
	v_TexCoord = texCoord; // 将输入的纹理坐标从texCoord传递到输出变量v_TexCoord，以便在片段着色器中使用。
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; // 定义一个输出变量color，它是vec4类型的（包含红色、绿色、蓝色和透明度分量）。layout(location = 0)指定这个输出变量的位置索引为0，这个索引在后续步骤中将用于把片段着色器的输出映射到帧缓冲对象的某个颜色缓冲区。

in vec2 v_TexCoord;

uniform vec4 u_Color; // 定义一个uniform变量u_Color，它是vec4类型的。uniform变量是在多个着色器之间共享的，它们通常用于传递全局数据（如光照信息、材质颜色等）。u_Color在这里可能被用于控制片段的颜色。
uniform sampler2D u_Texture; // 定义一个uniform变量u_Texture，它是sampler2D类型的。sampler2D用于在片段着色器中访问二维纹理。这里u_Texture将被用来从纹理中采样颜色。传入此参数时只需传入位于哪个slot。

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord); // 使用texture函数从u_Texture纹理中采样颜色。v_TexCoord是来自顶点着色器的纹理坐标，它指示了从纹理中采样哪个位置的颜色。采样得到的颜色被存储在变量texColor中。
	color = texColor; // 
};