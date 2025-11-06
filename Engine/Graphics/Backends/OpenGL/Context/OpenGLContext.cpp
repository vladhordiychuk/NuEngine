#include <Graphics/Backends/OpenGL/Context/OpenGLContext.hpp>

#if defined(_WIN32)
    #include <Graphics/Backends/OpenGL/Platform/Windows/OpenGLContextWin32.hpp>
#elif defined(__linux__)
    #include <Graphics/Backends/OpenGL/Platform/Linux/OpenGLContextX11.hpp>
#elif defined(__APPLE__)
    #include <Graphics/Backends/OpenGL/Platform/MacOS/OpenGLContextCocoa.hpp>
#endif

namespace NuEngine::Graphics::OpenGL
{
    Core::Result<std::unique_ptr<OpenGLContext>, Core::GraphicsError> CreatePlatformOpenGLContext(Platform::IWindow* window) noexcept
    {
#if defined(_WIN32)
        return Core::Ok<std::unique_ptr<OpenGLContext>, Core::GraphicsError>(std::make_unique<OpenGLContextWin32>(window));
#elif defined(__linux__)
        return Core::Ok<std::unique_ptr<OpenGLContext>, Core::GraphicsError>(std::make_unique<OpenGLContextX11>());
#elif defined(__APPLE__)
        return Core::Ok<std::unique_ptr<OpenGLContext>, Core::GraphicsError>(std::make_unique<OpenGLContextCocoa>());
#else
        return Core::Err<std::unique_ptr<OpenGLContext>, Core::GraphicsError>Core::GraphicsError::PlatformFailure);
#endif
    }
}
