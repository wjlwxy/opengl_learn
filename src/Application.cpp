
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

    return { ss[0].str(), ss[1].str()}; // 转成字符串再以结构体的形式返回，将自动构造结构体
}


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id =  glCreateShader(type); // 创建着色器
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // 为着色器附加源代码
    glCompileShader(id); // 编译着色器

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // alloca是C语言中用于在栈上开辟空间用的  使用堆区malloc也行，但是得注意释放资源
        /*const int len = length;
        char message[len];  即使使用const也不能初始化数组 */
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
    unsigned int program = glCreateProgram(); // 创建程序
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs); // 附加着色器到程序上
    glAttachShader(program, fs);
    glLinkProgram(program); // 链接程序
    glValidateProgram(program); // 验证程序

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
    // 如果是COMPAT也就是兼容模式，会创建（默认0）并自动绑定一个默认的顶点数组对象，而CORE则不会绑定，或者说CORE并不会主动创建一个顶点数组对象（虽然有初始值）
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

    // glfwSwapInterval 是 GLFW 库中的一个函数，用于设置垂直同步（VSync）的间隔。垂直同步是用于防止屏幕撕裂的技术，它通过将缓冲区交换操作与显示器的刷新率同步来确保图像的平滑显示。
    glfwSwapInterval(1); // 0 关闭； 1 开启； 2 开启但同步频率为1的两倍。

    /* glewInit 需要窗口上下文(context) */
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
        /* 设置顶点缓冲布局， 会绑定当前的一个顶点缓冲区 ，同时也会将顶点缓冲区与顶点数组对象绑定，使得后面只需要绑定顶点数组对象就行*/
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // 第一个0为顶点属性中位置属性的索引，在着色器中被指定为0   参数2为一个属性所占元素数量， 2 * sizeof(float)为一整组属性所占字节数， 最后的0是此属性的起始偏移

        IndexBuffer ib(indices, 6);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLCall(ShaderProgramSource source = ParseShader("res/shader/basic.shader"));
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        // uniform定义的变量，比如u_Color，如果未被使用，也会被opengl剥离，从而返回-1
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);

        // 解除绑定
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
            //设置顶点缓冲布局， 会绑定当前的一个顶点缓冲区
            GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
            GLCall(glBindVertexArray(vao));

            // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // nullptr表示索引数据是从当前绑定的IBO中获取的。

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

    glfwTerminate(); // 会破坏上下文context
    return 0;
}