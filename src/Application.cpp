
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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


    float positions[8] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
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
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        //VertexBuffer vb2(positions2, 4 * 2 * sizeof(float));
        VertexBufferLayout vblayout;
        vblayout.Push<float>(2);
        //vblayout.Push<float>(2);
        //GLCall(glEnableVertexAttribArray(0));
        /* ���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺���� ��ͬʱҲ�Ὣ���㻺�����붥���������󶨣�ʹ�ú���ֻ��Ҫ�󶨶�������������*/
        //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // ��һ��0Ϊ����������λ�����Ե�����������ɫ���б�ָ��Ϊ0   ����2Ϊһ��������ռԪ�������� 2 * sizeof(float)Ϊһ����������ռ�ֽ����� ����0�Ǵ����Ե���ʼƫ��

        /*unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));*/
        VertexArray va;
        va.AddBuffer(vb, vblayout);
        IndexBuffer ib(indices, 6); // ��Ҫ�Ȱ󶨶��������ٰ����������������ܳɹ��������������󶨵�����������
        //va.AddBuffer(vb2, vblayout);

        //IndexBuffer ib2(indices, 6); // ��Ҫ�Ȱ󶨶��������ٰ����������������ܳɹ��������������󶨵�����������

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        /*GLCall(ShaderProgramSource source = ParseShader("res/shader/basic.shader"));
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));*/

        Shader shader("res/shader/basic.shader");
        shader.Bind();


        // uniform����ı���������u_Color�����δ��ʹ��(ָ������ɫ��������ֻ�����˵���ʹ�ã���Ҳ�ᱻopengl���룬�Ӷ�����-1
        //GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        //ASSERT(location != -1);

        // �����
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.UnBind();

        float increment = 0.05f;
        float r = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            //GLCall(glUseProgram(shader));
            //GLCall(glUniform4f(location, r, 0.3f, 0.6f, 0.7f));
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.5f, 0.3f, 0.9f);

            /* GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            GLCall(glEnableVertexAttribArray(0));
            //���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺����
            GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
            //GLCall(glBindVertexArray(vao));
            va.Bind();

            // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // nullptr��ʾ���������Ǵӵ�ǰ�󶨵�IBO�л�ȡ�ġ�

            if (r < 0.0f)
                increment = 0.05f;
            if (r > 1.0f)
                increment = -0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        //GLCall(glDeleteProgram(shader));
    }

    glfwTerminate(); // ���ƻ�������context
    return 0;
}