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
        -0.5f, -0.5f, 0.0f, 0.0f, // ������(0.0f, 0.0f)����ô��ͼƬ�����꣨�������꣩�������ģ���������ôӦ�õĲ�̫��ȷ�������붥�������йأ�vblayout.Push��va.AddBuffer
        0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,

        -2.5f, -2.5f, 0.0f, 0.0f, // ������(0.0f, 0.0f)����ô��ͼƬ�����꣨�������꣩�������ģ���������ôӦ�õĲ�̫��ȷ�������붥�������йأ�vblayout.Push��va.AddBuffer
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

            /* �޷����Ƴ������ı��ε�ԭ��϶��������������й�   �������Render����Draw����ʱ����һ������ָ�����Ƶ��������� */
            m_Ib = std::make_unique<IndexBuffer>(indices, 12); // ��Ҫ�Ȱ󶨶��������ٰ����������������ܳɹ��������������󶨵�����������

            m_Proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f); // ��x����x����y, ��y, ��z, ��z�� xy�������ʾ��Ļ��С�����㻺�����������账�ڴ˷�Χ�ڣ������޷���ʾ
            m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0)); // view matrix: ���������ʵ������������
            // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0.5f, 0)); // model matrix�� �ƶ����壬����view matrix����     

            m_Shader = std::make_unique<Shader>("res/shader/basic.shader");
            m_Shader->Bind();

            // ��������
            m_Texture = std::make_unique<Texture>("res/textures/����.png");
            m_Texture->Bind(); // Bind��Ĭ��ֵ0��Ŀ����ָ�󶨵��ĸ�slot
            m_Shader->SetUniform1i("u_Texture", 0); // 0��λ�õ�ֵҪ���Ϸ�Bind��ʵ��һ��, u_Textureָ��basic.shader�е�u_Texture

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

            glm::mat4 modelA = glm::translate(glm::mat4(1.0f), m_TranslationA); // model matrix�� �ƶ����壬����view matrix����

            glm::mat4 mvpA = m_Proj * m_View * modelA; // ����˷�����Ϊopengl����������


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

