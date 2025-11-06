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
    Core::Result<std::unique_ptr<IWindow>, Core::WindowError> CreatePlatformWindow() noexcept
    {
#if defined(_WIN32)
        return Core::Ok<std::unique_ptr<IWindow>, Core::WindowError>(std::make_unique<WindowWin32>());
#elif defined(__linux__)
        return Core::Ok<std::unique_ptr<IWindow>, Core::WindowError>(std::make_unique<WindowX11>());
#elif defined(__APPLE__)
        return Core::Ok<std::unique_ptr<IWindow>, Core::WindowError>(std::make_unique<WindowCocoa>());
#else
        return NCore::Err<std::unique_ptr<IWindow>, Core::WindowError>(Core::WindowError::PlatformFailure);
#endif
    }
}
