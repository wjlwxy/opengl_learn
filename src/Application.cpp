
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 如果是COMPAT也就是兼容模式，会创建（默认0）并自动绑定一个默认的顶点数组对象，而CORE则不会绑定，或者说CORE并不会主动创建一个顶点数组对象（虽然有初始值）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // glfwSwapInterval 是 GLFW 库中的一个函数，用于设置垂直同步（VSync）的间隔。垂直同步是用于防止屏幕撕裂的技术，它通过将缓冲区交换操作与显示器的刷新率同步来确保图像的平滑显示。
    glfwSwapInterval(1); // 0 关闭； 1 开启； 2 开启但同步频率为1的两倍。

    /* glewInit 需要窗口上下文(context) */
    if (GLEW_OK != glewInit())
        std::cout << "GlewInit error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;


    float positions_texcoords[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, // 后方两个(0.0f, 0.0f)是怎么与图片的坐标（纹理坐标）所关联的，但具体怎么应用的不太明确，至少与顶点属性有关，vblayout.Push和va.AddBuffer
        0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f
    };

    /*float positions2[8] = {
        1.f, -0.5f,
        2.f, 0.5f,
        2.5f, -0.5f,
        1.f, 0.5f,
    };*/

    unsigned int indices[] = {
        0,1,2,
        0,1,3
    };




    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBuffer vb(positions_texcoords, 4 * 4 * sizeof(float));
        VertexBufferLayout vblayout;
        vblayout.Push<float>(2);
        vblayout.Push<float>(2);
        // GLCall(glEnableVertexAttribArray(0));
        /* 设置顶点缓冲布局， 会绑定当前的一个顶点缓冲区 ，同时也会将顶点缓冲区与顶点数组对象绑定，使得后面只需要绑定顶点数组对象就行*/
        // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // 第一个0为顶点属性中位置属性的索引，在着色器中被指定为0   参数2为一个属性所占元素数量， 2 * sizeof(float)为一整组属性所占字节数， 最后的0是此属性的起始偏移

        /*unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));*/
        VertexArray va;
        va.AddBuffer(vb, vblayout);
        IndexBuffer ib(indices, 6); // 需要先绑定顶点数组再绑定索引缓冲区，才能成功将索引缓冲区绑定到顶点数组上

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f); // 左x，右x，下y, 上y, 外z, 里z， xy的坐标表示屏幕大小，顶点缓冲区的坐标需处在此范围内，否则无法显示
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0)); // view matrix: 相机右移其实就是物体左移
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0.5f, 0)); // model matrix： 移动物体，但与view matrix反向

        // glm::mat4 mvp = proj * view * model; // 反向乘法是因为opengl中是列主向

        ///*GLCall(ShaderProgramSource source = ParseShader("res/shader/basic.shader"));
        //std::cout << "VERTEX" << std::endl;
        //std::cout << source.VertexSource << std::endl;
        //std::cout << "FRAGMENT" << std::endl;
        //std::cout << source.FragmentSource << std::endl;
        //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        //GLCall(glUseProgram(shader));*/

        Shader shader("res/shader/basic.shader");
        shader.Bind();
        // shader.SetUniform4f("u_Color", 0.1f, 0.5f, 0.3f, 0.9f);
        // shader.SetUniformMat4f("u_MVP", mvp);


        // uniform定义的变量，比如u_Color，如果未被使用(指的是着色器程序中只定义了但不使用），也会被opengl剥离，从而返回-1
        //GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        //ASSERT(location != -1);

        // 创建纹理
        Texture texture("res/textures/萱萱.png");
        texture.Bind(); // Bind有默认值0，目的是指绑定到哪个slot
        shader.SetUniform1i("u_Texture", 0); // 0的位置的值要与上方Bind的实参一致, u_Texture只是个名字（代号）而已

        // 解除绑定
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.UnBind();

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(2, 0.5f, 0);
        glm::vec3 translationB(0, -0.5f, 0);

        float increment = 0.05f;
        float r = 0.0f;

        Renderer render;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            render.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            {
                glm::mat4 modelA = glm::translate(glm::mat4(1.0f), translationA); // model matrix： 移动物体，但与view matrix反向

                glm::mat4 mvpA = proj * view * modelA; // 反向乘法是因为opengl中是列主向

                //glClear(GL_COLOR_BUFFER_BIT);

                //GLCall(glUseProgram(shader));
                //GLCall(glUniform4f(location, r, 0.3f, 0.6f, 0.7f));
                shader.Bind();
                //shader.SetUniform4f("u_Color", r, 0.5f, 0.3f, 0.9f);
                shader.SetUniformMat4f("u_MVP", mvpA);

                /* GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                GLCall(glEnableVertexAttribArray(0));
                //设置顶点缓冲布局， 会绑定当前的一个顶点缓冲区
                GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
                //GLCall(glBindVertexArray(vao));

                //va.Bind();

                // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

                //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // nullptr表示索引数据是从当前绑定的IBO中获取的。
                render.Draw(va, ib, shader);
            }

            {
                glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvpB = proj * view * modelB;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvpB);
                render.Draw(va, ib, shader);
            }




            if (r < 0.0f)
                increment = 0.05f;
            if (r > 1.0f)
                increment = -0.05f;

            r += increment;

            {
                //static float f = 0.0f;                        
                ImGui::SliderFloat3("translation A", &translationA.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
                ImGui::SliderFloat3("translation B", &translationB.x, -1.0f, 1.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        //GLCall(glDeleteProgram(shader));
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate(); // 会破坏上下文context
    return 0;
}