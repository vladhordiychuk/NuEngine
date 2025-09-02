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
    std::unique_ptr<IWindow> CreatePlatformWindow()
    {
#if defined(_WIN32)
        return std::make_unique<WindowWin32>();
#elif defined(__linux__)
        return std::make_unique<WindowX11>();
#elif defined(__APPLE__)
        return std::make_unique<WindowCocoa>();
#else
        #error "Unsupported platform!"
#endif
    }
}

