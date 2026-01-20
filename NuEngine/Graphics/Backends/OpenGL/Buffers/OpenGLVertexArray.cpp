#include <Graphics/Backends/OpenGL/Buffers/OpenGLVertexArray.hpp>
#include <Core/Logging/Logger.hpp>
#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer)
	{
		if (vertexBuffer->GetLayout().GetElements().empty())
		{
			LOG_ERROR("Vertex Buffer has no layout!");
			return;
		}

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		unsigned int index = 0;

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(uintptr_t)element.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);

		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		glBindVertexArray(0);
	}
}