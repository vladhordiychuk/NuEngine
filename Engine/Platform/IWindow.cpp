#include <Platform/IWindow.hpp>

#if defined(_WIN32)
    #include <Platform/Windows/WindowWin32.hpp>
#elif defined(__linux__)
    #include <Platform/Linux/WindowX11.hpp>
#elif defined(__APPLE__)
    #include <Platform/Mac/WindowCocoa.hpp>
#endif

namespace NuEngine::Platform
{
    NuEngine::Core::Result<std::unique_ptr<IWindow>, NuEngine::Core::WindowError> CreatePlatformWindow() noexcept
    {
#if defined(_WIN32)
        return NuEngine::Core::Ok<std::unique_ptr<IWindow>, NuEngine::Core::WindowError>(std::make_unique<WindowWin32>());
#elif defined(__linux__)
        return NuEngine::Core::Ok<std::unique_ptr<IWindow>, NuEngine::Core::WindowError>(std::make_unique<WindowX11>());
#elif defined(__APPLE__)
        return NuEngine::Core::Ok<std::unique_ptr<IWindow>, NuEngine::Core::WindowError>(std::make_unique<WindowCocoa>());
#else
        return NuEngine::Core::Err<std::unique_ptr<IWindow>, NuEngine::Core::WindowError>(Core::WindowError::PlatformFailure);
#endif
    }
}
