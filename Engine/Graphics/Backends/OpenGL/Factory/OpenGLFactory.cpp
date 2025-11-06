#include <Graphics/Backends/OpenGL/Factory/OpenGLFactory.hpp>
#include <Graphics/Backends/OpenGL/Context/OpenGLContext.hpp>
#include <Graphics/Backends/OpenGL/Device/OpenGLDevice.hpp>

namespace NuEngine::Graphics::OpenGL
{
    Core::Result<std::unique_ptr<IRenderDevice>, Core::GraphicsError>
        OpenGLFactory::CreateDevice(Platform::IWindow* window) noexcept
    {
        auto ctxResult = CreatePlatformOpenGLContext(window);
        if (ctxResult.IsError())
        {
            return Core::Err<std::unique_ptr<IRenderDevice>, Core::GraphicsError>(
                Core::GraphicsError::ContextCreationFailed
            );
        }

        auto ctx = std::move(ctxResult.Unwrap());

        auto initRes = ctx->Initialize();
        if (initRes.IsError())
        {
            return Core::Err<std::unique_ptr<IRenderDevice>, Core::GraphicsError>(
                Core::GraphicsError::FunctionLoadFailed
            );
        }

        return Core::Ok<std::unique_ptr<IRenderDevice>, Core::GraphicsError>(
            std::make_unique<OpenGLDevice>(std::move(ctx))
        );
    }

}