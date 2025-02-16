#pragma once

#include "Test.h"
#include <memory>

class Renderer;
class Shader;
class VertexArray;
class Texture;
class IndexBuffer;
class VertexBuffer;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
		glm::mat4 m_Proj;
		glm::mat4 m_View;

		std::unique_ptr<Renderer> m_Render;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_Va;
		std::unique_ptr<IndexBuffer> m_Ib;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_Vb;
	};
}