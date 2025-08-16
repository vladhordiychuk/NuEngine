// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <vector>
#include <glad/glad.h>

namespace NuEngine::Renderer::OpenGL
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