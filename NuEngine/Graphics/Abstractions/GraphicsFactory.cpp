#include <Graphics/Abstractions/GraphicsFactory.hpp>
#include <Graphics/Backends/OpenGL/Factory/OpenGLFactory.hpp>

namespace NuEngine::Graphics
{
    Core::Result<std::unique_ptr<IRenderDevice>, GraphicsError>
        GraphicsFactory::CreateDevice(GraphicsAPI api, Platform::IWindow* window) noexcept
    {
        switch (api)
        {
        case GraphicsAPI::OpenGL:
            return OpenGL::OpenGLFactory::CreateDevice(window);

        case GraphicsAPI::Vulkan:
        case GraphicsAPI::DirectX:
        default:
            return Core::Err(GraphicsError(GraphicsErrorCode::InvalidParameter));
        }
    }
}


