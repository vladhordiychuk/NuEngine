#include <Graphics/Backends/OpenGL/Core/OpenGLFactory.hpp>
#include <Graphics/Backends/OpenGL/Core/OpenGLContext.hpp>
#include <Graphics/Backends/OpenGL/Core/OpenGLDevice.hpp>

namespace NuEngine::Graphics::OpenGL
{
	Core::Result<std::unique_ptr<IRenderDevice>, GraphicsError>
		OpenGLFactory::CreateDevice(Platform::IWindow* window) noexcept
	{
		auto ctxResult = CreatePlatformOpenGLContext(window);
		if (ctxResult.IsError())
		{
			return Core::Err(ctxResult.UnwrapError());
		}

		auto ctx = std::move(ctxResult.Unwrap());

		auto initRes = ctx->Initialize();
		if (initRes.IsError())
		{
			return Core::Err(initRes.UnwrapError());
		}

		std::unique_ptr<IRenderDevice> device = std::make_unique<OpenGLDevice>(std::move(ctx));

		return Core::Ok(std::move(device));
	}
}