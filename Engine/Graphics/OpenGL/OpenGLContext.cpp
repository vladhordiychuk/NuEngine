#include <Graphics/OpenGL/OpenGLContext.hpp>

#if defined(_WIN32)
    #include <Platform/Windows/OpenGLContextWin32.hpp>
#elif defined(__linux__)
    #include <Platform/Linux/OpenGLContextX11.hpp>
#elif defined(__APPLE__)
    #include <Platform/MacOS/OpenGLContextCocoa.hpp>
#endif

namespace NuEngine::Graphics::OpenGL
{
    NuEngine::Core::Result<std::unique_ptr<OpenGLContext>, NuEngine::Core::GraphicsError> CreatePlatformOpenGLContext(Platform::IWindow* window) noexcept
    {
#if defined(_WIN32)
        return NuEngine::Core::Ok<std::unique_ptr<OpenGLContext>, NuEngine::Core::GraphicsError>(std::make_unique<Platform::OpenGLContextWin32>(window));
#elif defined(__linux__)
        return NuEngine::Core::Ok<std::unique_ptr<OpenGLContext>, NuEngine::Core::GraphicsError>(std::make_unique<Platform::OpenGLContextX11>());
#elif defined(__APPLE__)
        return NuEngine::Core::Ok<std::unique_ptr<OpenGLContext>, NuEngine::Core::GraphicsError>(std::make_unique<Platform::OpenGLContextCocoa>());
#else
        return NuEngine::Core::Err<std::unique_ptr<OpenGLContext>, NuEngine::Core::GraphicsError>(NuEngine::Core::GraphicsError::PlatformFailure);
#endif
    }
}
