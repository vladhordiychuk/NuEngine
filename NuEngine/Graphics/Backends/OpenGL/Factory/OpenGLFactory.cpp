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
            return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));
        }

        auto ctx = std::move(ctxResult.Unwrap());

        auto initRes = ctx->Initialize();
        if (initRes.IsError())
        {
            return Core::Err(GraphicsError(GraphicsErrorCode::FunctionLoadFailed));
        }

        // ВАЖЛИВО: Явно вказуємо базовий тип (IRenderDevice), а не auto.
        // Це дозволяє Core::Ok створити правильну обгортку, яку очікує Result.

        std::unique_ptr<IRenderDevice> device = std::make_unique<OpenGLDevice>(std::move(ctx));

        return Core::Ok(std::move(device));
    }
}