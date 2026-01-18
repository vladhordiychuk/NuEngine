#include <Renderer/Pipeline/ForwardPipeline.hpp>

#include <NuMath/NuMath.hpp>

namespace NuEngine::Renderer
{
	ForwardPipeline::ForwardPipeline(Graphics::IRenderDevice* device)
		: m_Device(device)
	{
	}

	Core::Result<void, Graphics::GraphicsError> ForwardPipeline::Render() noexcept
	{
		NuMath::Color color = NuMath::Colors::Linear::Green;

		NuMath::Vector4 vec4 = { 0.0f, 0.0f, 1.0f, 2.0f };

		NuMath::Matrix4x4 mat4 = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};


		if (!m_Device)
		{
			return Core::Err(Graphics::GraphicsError(Graphics::GraphicsErrorCode::InvalidContext));
		}
		m_Device->Clear(color);

		auto drawResult = m_Device->DrawTriangle();
		if (drawResult.IsError())
		{
			return drawResult;
		}

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