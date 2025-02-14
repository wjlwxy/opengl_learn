
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

#include "tests/TestClearColor.h"

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

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        //test::TestClearColor test; 

        Renderer render;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            render.SetClearColor(0.9f, 0.4f, 1.0f, 1.0f);
            render.Clear();

            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-")) // ���߱�ʾ�Ƿ��ڲ˵����棬���˳��˲˵����棬������currentTest
                {
                    delete currentTest; // delete�Ķ������� test->RegisterTest �� new ������
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }
           

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate(); // ���ƻ�������context
    return 0;
}