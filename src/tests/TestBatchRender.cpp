#include "TestBatchRender.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{

    static std::array<Vertex, 4> CreateQuad(float x, float y, float texTureID)
    {
        float size = 100.0f;

        float pos0[] = { x, y, 0.0f };
        float color0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
        float texcoords0[] = { 0.0f, 0.0f };

        Vertex v0(pos0, color0, texcoords0, texTureID);

        float pos1[] = { x + size, y, 0.0f };
        float color1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
        float texcoords1[] = { 1.0f, 0.0f };

        Vertex v1(pos1, color1, texcoords1, texTureID);

        float pos2[] = { x + size, y + size, 0.0f };
        float color2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
        float texcoords2[] = { 1.0f, 1.0f };

        Vertex v2(pos2, color2, texcoords2, texTureID);

        float pos3[] = { x, y + size, 0.0f };
        float color3[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        float texcoords3[] = { 0.0f, 1.0f };

        Vertex v3(pos3, color3, texcoords3, texTureID);

        return { v0, v1, v2, v3 };
    }

    /*static Vertex* CreateQuad(Vertex* target, float x, float y, float texTureID)
    {
        float size = 100.0f;

        float pos0[] = { x, y, 0.0f };
        float color0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
        float texcoords0[] = { 0.0f, 0.0f };

        Vertex v0(pos0, color0, texcoords0, texTureID);
        *target = v0;
        target++;

        float pos1[] = { x + size, y, 0.0f };
        float color1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
        float texcoords1[] = { 1.0f, 0.0f };

        Vertex v1(pos1, color1, texcoords1, texTureID);
        *target = v1;
        target++;

        float pos2[] = { x + size, y + size, 0.0f };
        float color2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
        float texcoords2[] = { 1.0f, 1.0f };

        Vertex v2(pos2, color2, texcoords2, texTureID);
        *target = v2;
        target++;

        float pos3[] = { x, y + size, 0.0f };
        float color3[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        float texcoords3[] = { 0.0f, 1.0f };

        Vertex v3(pos3, color3, texcoords3, texTureID);
        *target = v3;
        target++;

        return target;
    }*/

    /*static void change_indices(std::vector<unsigned int> array)
    {
        ImGui::Text("input indices:");
        for (int i = 0; i < array.size(); ++i) {
            ImGui::InputInt(std::string("array[" + std::to_string(i) + "]").c_str(), (int*)&array[i]);
        }
    }*/

    TestBatchRender::TestBatchRender()
        :m_Proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
        m_Translation(glm::vec3(0, 0, 0)),
        m_MaxQuadCount(26),
        m_MaxVertexCount(m_MaxQuadCount * 4),
        m_MaxIndexCount(m_MaxQuadCount * 6)
    {
        /*float positions[] = {
            100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            200.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            300.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            400.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            300.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f

        };*/

        /*unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };*/

        /*Vertex* buffer = m_vertices.data();
        for (int y = 0; y < 200; y += 100)
        {
            for (int x = 0; x < 200; x += 100) 
            {
                buffer = CreateQuad(buffer, x, y, ((x + y) / 100) % 2);
                m_IndiceCount += 6;
            }
        }

        buffer = CreateQuad(buffer, m_QuadPositionA[0], m_QuadPositionA[0], 0.0f);
        m_IndiceCount += 6;

        for (size_t i = 0; i < m_IndiceCount; i += 6)
        {
            m_Indices[i + 0] = 0 + m_offset;
            m_Indices[i + 1] = 1 + m_offset;
            m_Indices[i + 2] = 2 + m_offset;

            m_Indices[i + 3] = 2 + m_offset;
            m_Indices[i + 4] = 3 + m_offset;
            m_Indices[i + 5] = 0 + m_offset;

            m_offset += 4;

        }*/

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();

        //m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 11 * 8 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, m_MaxVertexCount * sizeof(Vertex));
        VertexBufferLayout layout;
        layout.Push<float>(sizeof(Vertex::Position)/sizeof(float)); // 坐标 x, y, z, w  w 齐次坐标, 对xyz进行缩放
        layout.Push<float>(sizeof(Vertex::Color) / sizeof(float)); // 颜色数据
        layout.Push<float>(sizeof(Vertex::TexCoords) / sizeof(float)); // 纹理坐标
        layout.Push<float>(sizeof(Vertex::TexID) / sizeof(float)); // 纹理插槽
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        //m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shader/Batch.shader");
        m_Shader->Bind();

        m_Texture[0] = std::make_unique<Texture>("res/textures/萱萱.png");
        m_Texture[1] = std::make_unique<Texture>("res/textures/1368.png");
        for (size_t i = 0; i < 2; i++)
        {
            m_Texture[i]->Bind(i);
        }
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers); // 将smaplers赋给Batch.shader中的u_Textures
    }

    TestBatchRender::~TestBatchRender()
    {
    }

    void TestBatchRender::OnUpdate(float deltaTime)
    {
        // 设置动态顶点缓冲区
        float positions[] = {
            100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            200.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            200.0f, 200.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            100.0f, 200.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            300.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            400.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            400.0f, 200.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            300.0f, 200.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f

        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        m_IndexBuffer = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());
        //m_IndexBuffer = std::make_unique<IndexBuffer>(&m_indices[0], m_IndiceCount);

        auto q0 = CreateQuad(m_QuadPositionA[0], m_QuadPositionA[1], m_TextureA);
        auto q1 = CreateQuad(m_QuadPositionB[0], m_QuadPositionB[1], m_TextureB);

        Vertex vertices[8];
        memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

        m_VertexBuffer->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
        /*GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_IndiceCount/6 * 4 * sizeof(Vertex), m_vertices.data()));
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size(), &m_indices[0]));*/
    }

    void TestBatchRender::OnRender()
    {
        GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer; // 每帧这个renderer都要不一样嘛
        glm::mat4 mvp = m_Proj * m_View;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void TestBatchRender::OnImGuiRender()
    {
        ImGui::Begin("Controls");
        ImGui::DragFloat2("Quad Position A", m_QuadPositionA, 10.0f);
        ImGui::DragFloat2("Quad Position B", m_QuadPositionB, 10.0f);
        ImGui::DragFloat("Texture A", &m_TextureA, 1.0f, 0.0f, 1.0f);
        ImGui::DragFloat("Texture B", &m_TextureB, 1.0f, 0.0f, 1.0f);
        /*if (ImGui::Button("change indices")) {
            change_indices(m_indices);
        }*/
        ImGui::End();
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}