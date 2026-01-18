// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Platform/Errors/WindowError.hpp>
#include <Platform/Common/WindowConfig.hpp>
#include <Platform/Common/WindowEvents.hpp>
#include <memory>

namespace NuEngine::Platform
{
    enum class NativeHandleType
    {
        Window,   // HWND / X11Window / NSWindow
        Display,  // HDC / X11Display / NSView
    };

    /*
    * @brief Abstract interface for platform-specific window management.
    *
    * The IWindow class defines a platform-agnostic interface for creating and managing
    * windows in the NuEngine. It provides methods for initialization, event handling,
    * rendering, and window configuration. Concrete implementations are provided for
    * different platforms (e.g., Windows, Linux, macOS).
    */
    class IWindow
    {
    public:
        /*
        * @brief Virtual destructor to ensure proper cleanup in derived classes.
        */
        virtual ~IWindow() = default;

        /*
        * @brief Initializes the window with the specified configuration.
        *
        * @param config The configuration parameters (e.g., title, size, VSync).
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> Initialize(const WindowConfig& config) = 0;

        /*
        * @brief Shuts down the window and releases associated resources.
        *
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> Shutdown() = 0;

        /*
        * @brief Makes the window visible on the screen.
        *
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> Show() = 0;

        /*
        * @brief Hides the window from the screen.
        *
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> Hide() = 0;

        /*
        * @brief Sets focus to the window.
        *
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> Focus() = 0;

        /*
        * @brief Processes window events (e.g., keyboard, mouse, window close).
        *
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> ProcessEvents() = 0;

        /*
        * @brief Swaps the front and back buffers for rendering (double buffering).
        *
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> SwapBuffers() = 0;

        /*
        * @brief Retrieves the platform-specific native window handle.
        *
        * @return void* Pointer to the native window handle (e.g., HWND on Windows, Window on X11).
        */
        virtual void* GetNativeHandle(NativeHandleType type) const = 0;

        /*
        * @brief Gets the current window configuration.
        *
        * @return WindowConfig The current configuration (e.g., title, size, VSync).
        */
        virtual WindowConfig GetConfig() const = 0;

        /*
        * @brief Checks if the window is open.
        *
        * @return bool True if the window is open, false otherwise.
        */
        [[nodiscard]] virtual bool IsOpen() const = 0;

        /*
        * @brief Checks if the window is currently focused.
        *
        * @return bool True if the window is focused, false otherwise.
        */
        [[nodiscard]] virtual bool IsFocused() const = 0;

        /*
        * @brief Sets the window title.
        *
        * @param title The new title for the window.
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> SetTitle(const std::string& title) = 0;

        /*
        * @brief Sets the window size.
        *
        * @param width The new width of the window in pixels.
        * @param height The new height of the window in pixels.
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> SetSize(int width, int height) = 0;

        /*
        * @brief Sets the window position on the screen.
        *
        * @param x The x-coordinate of the window's top-left corner.
        * @param y The y-coordinate of the window's top-left corner.
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> SetPosition(int x, int y) = 0;

        /*
        * @brief Enables or disables vertical synchronization (VSync).
        *
        * @param vsync True to enable VSync, false to disable.
        * @return Result<void, WindowError> Returns Ok on success or an error code on failure.
        */
        virtual Core::Result<void, WindowError> SetVSync(bool vsync) = 0;

        /*
        * @brief Retrieves the event system for handling window events.
        *
        * @return WindowEventSystem& Reference to the event system for processing window events.
        */
        virtual WindowEventSystem& GetEventSystem() = 0;

        virtual void SetEventCallback(const std::function<void(WindowEvent&)>& callback) = 0;
    };

    /*
    * @brief Factory function to create a platform-specific window instance.
    *
    * Creates a platform-specific implementation of IWindow based on the current operating system
    * (e.g., WindowWin32 for Windows, WindowX11 for Linux, WindowCocoa for macOS).
    *
    * @return Result<std::unique_ptr<IWindow>, WindowError> Returns a unique pointer to the
    * window instance on success or an error code on failure.
    */
    Core::Result<std::unique_ptr<IWindow>, WindowError> CreatePlatformWindow() noexcept;
}