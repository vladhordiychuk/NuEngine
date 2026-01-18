#include <Graphics/Backends/OpenGL/Factory/OpenGLFactory.hpp>
#include <Graphics/Backends/OpenGL/Context/OpenGLContext.hpp>
#include <Graphics/Backends/OpenGL/Device/OpenGLDevice.hpp>

namespace NuEngine::Graphics::OpenGL
{
	Core::Result<std::unique_ptr<IRenderDevice>, GraphicsError>
		OpenGLFactory::CreateDevice(Platform::IWindow* window) noexcept
	{
		auto ctxResult = CreatePlatformOpenGLContext(window);
		if (ctxResult.IsError())
		{
			// Тут ви теж "ковтали" помилку створення контексту. Виправляємо:
			return Core::Err(ctxResult.UnwrapError());
		}

		auto ctx = std::move(ctxResult.Unwrap());

		auto initRes = ctx->Initialize();
		if (initRes.IsError())
		{
			// !!! ВИПРАВЛЕННЯ ТУТ !!!
			// Ми повертаємо саму помилку initRes, яка містить текст "GLAD init failed..."
			return Core::Err(initRes.UnwrapError());
		}

		std::unique_ptr<IRenderDevice> device = std::make_unique<OpenGLDevice>(std::move(ctx));

		return Core::Ok(std::move(device));
	}
}