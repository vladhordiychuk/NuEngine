// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/Buffers/IIndexBuffer.hpp>

namespace NuEngine::Graphics::OpenGL
{
	typedef unsigned int GLuint;

	class OpenGLIndexBuffer : public IIndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		unsigned int GetCount() const override { return m_Count; }

	private:
		GLuint m_RendererID;
		unsigned int m_Count;
	};
}