#include "TestTexture2D.h"

#include <GL/glew.h>
#include "Renderer.h"

#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"

namespace test {

    TestTexture2D::TestTexture2D()
        :m_TranslationA({ 2, 0.5f, 0 }), m_TranslationB{ 2, 0.5f, 0 }
    {
        float positions_texcoords[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, // 后方两个(0.0f, 0.0f)是怎么与图片的坐标（纹理坐标）所关联的，但具体怎么应用的不太明确，至少与顶点属性有关，vblayout.Push和va.AddBuffer
        0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,

        -2.5f, -2.5f, 0.0f, 0.0f, // 后方两个(0.0f, 0.0f)是怎么与图片的坐标（纹理坐标）所关联的，但具体怎么应用的不太明确，至少与顶点属性有关，vblayout.Push和va.AddBuffer
        -1.5f, -1.5f, 1.0f, 1.0f,
        -1.5f, -2.5f, 1.0f, 0.0f,
        -2.5f, -1.5f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            4,5,6,
            4,5,7,
            
            0,1,2,
            0,1,3
        };

        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

            m_Vb = std::make_unique<VertexBuffer>(positions_texcoords, 8 * 4 * sizeof(float));
            VertexBufferLayout vblayout;
            vblayout.Push<float>(2);
            vblayout.Push<float>(2);

            m_Va = std::make_unique<VertexArray>();
            m_Va->AddBuffer(*m_Vb, vblayout);

            /* 无法绘制出两个四边形的原因肯定跟索引缓冲区有关   结果：在Render调用Draw方法时会有一个参数指定绘制的索引数量 */
            m_Ib = std::make_unique<IndexBuffer>(indices, 12); // 需要先绑定顶点数组再绑定索引缓冲区，才能成功将索引缓冲区绑定到顶点数组上

            m_Proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f); // 左x，右x，下y, 上y, 外z, 里z， xy的坐标表示屏幕大小，顶点缓冲区的坐标需处在此范围内，否则无法显示
            m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0)); // view matrix: 相机右移其实就是物体左移
            // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0.5f, 0)); // model matrix： 移动物体，但与view matrix反向     

            m_Shader = std::make_unique<Shader>("res/shader/basic.shader");
            m_Shader->Bind();

            // 创建纹理
            m_Texture = std::make_unique<Texture>("res/textures/萱萱.png");
            m_Texture->Bind(); // Bind有默认值0，目的是指绑定到哪个slot
            m_Shader->SetUniform1i("u_Texture", 0); // 0的位置的值要与上方Bind的实参一致, u_Texture指代basic.shader中的u_Texture

            m_Render = std::make_unique<Renderer>();
        }
    }

    TestTexture2D::~TestTexture2D()
    {

    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.8f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            glm::mat4 modelA = glm::translate(glm::mat4(1.0f), m_TranslationA); // model matrix： 移动物体，但与view matrix反向

            glm::mat4 mvpA = m_Proj * m_View * modelA; // 反向乘法是因为opengl中是列主向


            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvpA);

            m_Render->Draw(*m_Va, *m_Ib, *m_Shader);
        }

        /*{
            glm::mat4 modelB = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvpB = m_Proj * m_View * modelB;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvpB);
            m_Render->Draw(*m_Va, *m_Ib, *m_Shader);
        }*/
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("translation A", &m_TranslationA.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        // ImGui::SliderFloat3("translation B", &m_TranslationB.x, -1.0f, 1.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}

