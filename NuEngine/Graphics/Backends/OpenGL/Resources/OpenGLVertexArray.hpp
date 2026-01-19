// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IVertexArray.hpp>
#include <vector>

namespace NuEngine::Graphics::OpenGL
{
	class OpenGLVertexArray : public IVertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) override;

		const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffer() const override
		{
			return m_VertexBuffers;
		}

		const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const override
		{
			return m_IndexBuffer;
		}

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<IVertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IIndexBuffer> m_IndexBuffer;
	};
}