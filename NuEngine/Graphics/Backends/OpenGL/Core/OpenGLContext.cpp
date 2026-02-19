#include <Graphics/Backends/OpenGL/Core/OpenGLContext.hpp>
#include <Graphics/Backends/OpenGL/Core/OpenGLContextExternal.hpp>

#if defined(_WIN32)
#include <Graphics/Backends/OpenGL/Platform/Windows/OpenGLContextWin32.hpp>
#elif defined(__linux__)
#include <Graphics/Backends/OpenGL/Platform/Linux/OpenGLContextX11.hpp>
#elif defined(__APPLE__)
#include <Graphics/Backends/OpenGL/Platform/MacOS/OpenGLContextCocoa.hpp>
#endif

namespace NuEngine::Graphics::OpenGL
{
    Core::Result<std::unique_ptr<OpenGLContext>, GraphicsError> CreatePlatformOpenGLContext(Platform::IWindow* window) noexcept
    {
        if (window == nullptr)
        {
            // 1. Створюємо derived вказівник і одразу зберігаємо як base
            std::unique_ptr<OpenGLContext> externalCtx = std::make_unique<OpenGLExternalContext>();

            // 2. Переміщуємо (move) вже правильний тип
            return Core::Ok(std::move(externalCtx));
        }

        std::unique_ptr<OpenGLContext> context;

#if defined(_WIN32)
        context = std::make_unique<OpenGLContextWin32>(window);
#elif defined(__linux__)
        context = std::make_unique<OpenGLContextX11>();
#elif defined(__APPLE__)
        context = std::make_unique<OpenGLContextCocoa>();
#else
        return Core::Err(GraphicsError(GraphicsErrorCode::PlatformFailure));
#endif
        return Core::Ok(std::move(context));
    }
}