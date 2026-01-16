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
    Core::Result<std::unique_ptr<IWindow>, WindowError> CreatePlatformWindow() noexcept
    {
        // 1. Створюємо змінну базового типу
        std::unique_ptr<IWindow> window;

#if defined(_WIN32)
        window = std::make_unique<WindowWin32>();
#elif defined(__linux__)
        window = std::make_unique<WindowX11>();
#elif defined(__APPLE__)
        window = std::make_unique<WindowCocoa>();
#else
        // Виправлено NCore -> Core
        return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
#endif

        // 2. Повертаємо вже правильний тип через std::move
        return Core::Ok(std::move(window));
    }
}