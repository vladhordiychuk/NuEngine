#pragma once

#include <vector>
#include <glad/glad.h>

namespace Engine::Renderer::OpenGL
{
	class Mesh
	{
	public:
		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		void Render() const;

	private:
		GLuint m_VAO, m_VBO, m_EBO;
		size_t m_IndexCount;
	};
}