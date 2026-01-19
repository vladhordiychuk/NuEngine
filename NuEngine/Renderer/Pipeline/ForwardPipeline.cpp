#include <Renderer/Pipeline/ForwardPipeline.hpp>
#include <NuMath/NuMath.hpp>
#include <Core/Logging/Logger.hpp>

namespace NuEngine::Renderer
{
	const char* k_VertexShader = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		void main() {
			gl_Position = vec4(aPos, 1.0);
		}
	)";

	const char* k_FragmentShader = R"(
		#version 330 core
		out vec4 FragColor;
		void main() {
			FragColor = vec4(0.2, 0.8, 0.3, 1.0);
		}
	)";

	ForwardPipeline::ForwardPipeline(Graphics::IRenderDevice* device)
		: m_Device(device)
	{
		Initialize();
	}

	void ForwardPipeline::Initialize()
	{
		if (!m_Device)
		{
			LOG_ERROR("ForwardPipeline: Invalid Device Context");
			return;
		}

		auto shaderRes = m_Device->CreateShader(k_VertexShader, k_FragmentShader);

		if (shaderRes.IsOk())
		{
			m_Shader = shaderRes.Unwrap();
		}
		else
		{
			LOG_ERROR("Critical: Failed to create shader! Reason: {}", shaderRes.UnwrapError().ToString());
		}

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_QuadVAO = m_Device->CreateVertexArray();

		auto vbo = m_Device->CreateVertexBuffer(vertices, sizeof(vertices));

		Graphics::BufferLayout layout = {
			{ Graphics::ShaderDataType::Float3, "aPos" }
		};
		vbo->SetLayout(layout);

		m_QuadVAO->AddVertexBuffer(vbo);

		auto ibo = m_Device->CreateIndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));
		m_QuadVAO->SetIndexBuffer(ibo);
	}

	Core::Result<void, Graphics::GraphicsError> ForwardPipeline::Render() noexcept
	{
		if (!m_Device)
		{
			return Core::Err(Graphics::GraphicsError(Graphics::GraphicsErrorCode::InvalidContext));
		}

		m_Device->Clear(NuMath::Color(0.1f, 0.1f, 0.1f, 1.0f));

		if (m_Shader)
		{
			m_Shader->Bind();
		}
		else
		{
			return Core::Ok();
		}

		if (m_QuadVAO)
		{
			auto result = m_Device->DrawIndices(m_QuadVAO);
			if (result.IsError()) return result;
		}

		m_Shader->Unbind();

		return m_Device->Present();
	}

	void ForwardPipeline::SetViewport(int x, int y, int width, int height) noexcept
	{
		if (m_Device)
		{
			m_Device->SetViewport(x, y, width, height);
		}
	}
}