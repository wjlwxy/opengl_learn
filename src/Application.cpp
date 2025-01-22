
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("Vertex") != std::string::npos)
                type = ShaderType::VERTEX;//set mode to Vertex
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;// set mode to fragment
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str()}; // ת���ַ������Խṹ�����ʽ���أ����Զ�����ṹ��
}


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id =  glCreateShader(type); // ������ɫ��
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // Ϊ��ɫ������Դ����
    glCompileShader(id); // ������ɫ��

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // alloca��C������������ջ�Ͽ��ٿռ��õ�  ʹ�ö���mallocҲ�У����ǵ�ע���ͷ���Դ
        /*const int len = length;
        char message[len];  ��ʹʹ��constҲ���ܳ�ʼ������ */
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Fail to compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& VertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); // ��������
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs); // ������ɫ����������
    glAttachShader(program, fs);
    glLinkProgram(program); // ���ӳ���
    glValidateProgram(program); // ��֤����

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

    unsigned int indices[] = {
        0,1,2,
        0,1,3
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    {
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        GLCall(glEnableVertexAttribArray(0));
        /* ���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺���� ��ͬʱҲ�Ὣ���㻺�����붥���������󶨣�ʹ�ú���ֻ��Ҫ�󶨶�������������*/
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // ��һ��0Ϊ����������λ�����Ե�����������ɫ���б�ָ��Ϊ0   ����2Ϊһ��������ռԪ�������� 2 * sizeof(float)Ϊһ����������ռ�ֽ����� ����0�Ǵ����Ե���ʼƫ��

        IndexBuffer ib(indices, 6);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLCall(ShaderProgramSource source = ParseShader("res/shader/basic.shader"));
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        // uniform����ı���������u_Color�����δ��ʹ�ã�Ҳ�ᱻopengl���룬�Ӷ�����-1
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);

        // �����
        GLCall(glBindVertexArray(0));
        vb.Unbind();
        ib.Unbind();
        GLCall(glUseProgram(0));

        float increment = 0.05f;
        float r = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.6f, 0.7f));

            /* GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            GLCall(glEnableVertexAttribArray(0));
            //���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺����
            GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
            GLCall(glBindVertexArray(vao));

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

        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate(); // ���ƻ�������context
    return 0;
}