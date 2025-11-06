#include <Renderer/Pipeline/ForwardPipeline.hpp>

namespace NuEngine::Renderer
{
	ForwardPipeline::ForwardPipeline(Graphics::IRenderDevice* device)
		: m_Device(device)
	{
	}

	Core::Result<void, Core::GraphicsError> ForwardPipeline::Render() noexcept
	{
		if (!m_Device)
		{
			return Core::Err(Core::GraphicsError::InvalidContext);
		}
		m_Device->Clear(1.0f, 1.0f, 1.0f, 0.0f);
		return m_Device->Present();
	}
}