#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "imgui/imgui.h"

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

#include <memory>
#include <vector>
#include <array>

namespace test
{
	struct Vertex
	{
		float Position[3];
		float Color[4];
		float TexCoords[2];
		float TexID;

		Vertex(float position[3], float color[4], float texcoords[2], float texID)
			: Position{ position[0], position[1], position[2] }, Color{ color[0], color[1], color[2], color[3] }, TexCoords{ texcoords[0], texcoords[1] }, TexID(texID)
		{
		}

		// 默认构造函数 数组必须被初始化，所以需要提供默认构造
		Vertex()
			: Position{ 0.0f, 0.0f, 0.0f },
			Color{ 0.0f, 0.0f, 0.0f, 1.0f },
			TexCoords{ 0.0f, 0.0f },
			TexID(0.0f)
		{
		}
	};

	class TestBatchRender : public Test
	{
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture[2];

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;

		const size_t m_MaxQuadCount;
		const size_t m_MaxVertexCount;
		const size_t m_MaxIndexCount;

		float m_QuadPositionA[2] = { 100.0f, 100.0f };
		float m_QuadPositionB[2] = { 300.0f, 100.0f };

		float m_TextureA = 0.0f;
		float m_TextureB = 1.0f;

		std::vector<unsigned int> m_indices= { 0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4 };

		uint32_t m_Indices[1000 * 6];
		uint32_t m_offset = 0;
		std::array<Vertex, 1000> m_vertices;
		uint32_t m_IndiceCount = 0;

	public:
		TestBatchRender();
		~TestBatchRender();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}