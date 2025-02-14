
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
    // �����COMPATҲ���Ǽ���ģʽ���ᴴ����Ĭ��0�����Զ���һ��Ĭ�ϵĶ���������󣬶�CORE�򲻻�󶨣�����˵CORE��������������һ���������������Ȼ�г�ʼֵ��
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

    // glfwSwapInterval �� GLFW ���е�һ���������������ô�ֱͬ����VSync���ļ������ֱͬ�������ڷ�ֹ��Ļ˺�ѵļ�������ͨ����������������������ʾ����ˢ����ͬ����ȷ��ͼ���ƽ����ʾ��
    glfwSwapInterval(1); // 0 �رգ� 1 ������ 2 ������ͬ��Ƶ��Ϊ1��������

    /* glewInit ��Ҫ����������(context) */
    if (GLEW_OK != glewInit())
        std::cout << "GlewInit error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;


    float positions_texcoords[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, // ������(0.0f, 0.0f)����ô��ͼƬ�����꣨�������꣩�������ģ���������ôӦ�õĲ�̫��ȷ�������붥�������йأ�vblayout.Push��va.AddBuffer
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
        /* ���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺���� ��ͬʱҲ�Ὣ���㻺�����붥���������󶨣�ʹ�ú���ֻ��Ҫ�󶨶�������������*/
        // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // ��һ��0Ϊ����������λ�����Ե�����������ɫ���б�ָ��Ϊ0   ����2Ϊһ��������ռԪ�������� 2 * sizeof(float)Ϊһ����������ռ�ֽ����� ����0�Ǵ����Ե���ʼƫ��

        /*unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));*/
        VertexArray va;
        va.AddBuffer(vb, vblayout);
        IndexBuffer ib(indices, 6); // ��Ҫ�Ȱ󶨶��������ٰ����������������ܳɹ��������������󶨵�����������

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f); // ��x����x����y, ��y, ��z, ��z�� xy�������ʾ��Ļ��С�����㻺�����������账�ڴ˷�Χ�ڣ������޷���ʾ
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0)); // view matrix: ���������ʵ������������
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0.5f, 0)); // model matrix�� �ƶ����壬����view matrix����

        // glm::mat4 mvp = proj * view * model; // ����˷�����Ϊopengl����������

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


        // uniform����ı���������u_Color�����δ��ʹ��(ָ������ɫ��������ֻ�����˵���ʹ�ã���Ҳ�ᱻopengl���룬�Ӷ�����-1
        //GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        //ASSERT(location != -1);

        // ��������
        Texture texture("res/textures/����.png");
        texture.Bind(); // Bind��Ĭ��ֵ0��Ŀ����ָ�󶨵��ĸ�slot
        shader.SetUniform1i("u_Texture", 0); // 0��λ�õ�ֵҪ���Ϸ�Bind��ʵ��һ��, u_Textureֻ�Ǹ����֣����ţ�����

        // �����
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
                glm::mat4 modelA = glm::translate(glm::mat4(1.0f), translationA); // model matrix�� �ƶ����壬����view matrix����

                glm::mat4 mvpA = proj * view * modelA; // ����˷�����Ϊopengl����������

                //glClear(GL_COLOR_BUFFER_BIT);

                //GLCall(glUseProgram(shader));
                //GLCall(glUniform4f(location, r, 0.3f, 0.6f, 0.7f));
                shader.Bind();
                //shader.SetUniform4f("u_Color", r, 0.5f, 0.3f, 0.9f);
                shader.SetUniformMat4f("u_MVP", mvpA);

                /* GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                GLCall(glEnableVertexAttribArray(0));
                //���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺����
                GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
                //GLCall(glBindVertexArray(vao));

                //va.Bind();

                // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

                //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // nullptr��ʾ���������Ǵӵ�ǰ�󶨵�IBO�л�ȡ�ġ�
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
    glfwTerminate(); // ���ƻ�������context
    return 0;
}