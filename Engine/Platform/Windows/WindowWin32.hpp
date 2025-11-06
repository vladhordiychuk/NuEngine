// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Platform/IWindow.hpp>
#include <Platform/Common/WindowEvents.hpp>
#include <windows.h>

namespace NuEngine::Platform
{
    using NuInt32 = NuEngine::Core::Types::NuInt32;
    using NuBool = NuEngine::Core::Types::NuBool;

    /*
    * @brief Windows-specific implementation of the IWindow interface.
    *
    * This class provides a Win32 API-based realization of the window management system.
    * It handles window creation, event processing, and rendering buffer swapping using
    * native Windows APIs. It supports double buffering and integrates with the event
    * system for handling keyboard, mouse, and window lifecycle events.
    */
    class WindowWin32 final : public IWindow
    {
    public:
        /*
        * @brief Default constructor for initializing window resources.
        */
        WindowWin32();

        /*
        * @brief Destructor that calls Shutdown() to release resources.
        */
        ~WindowWin32() override;

        /*
        * @brief Initializes the window with the specified configuration using Win32 APIs.
        *
        * Registers the window class, creates the window handle, and sets up the device context.
        *
        * @param config The configuration parameters (e.g., title, size, VSync).
        * @return Result<void, Core::WindowError> Returns Ok on success or an error code on failure.
        */
        Core::Result<void, Core::WindowError> Initialize(const WindowConfig& config) override;

        /*
        * @brief Shuts down the window and releases Win32 resources (e.g., DC, HWND).
        */
        Core::Result<void, Core::WindowError>  Shutdown() override;

        /*
        * @brief Makes the window visible using ShowWindow API.
        */
        Core::Result<void, Core::WindowError>  Show() override;

        /*
        * @brief Hides the window using ShowWindow API.
        */
        Core::Result<void, Core::WindowError>  Hide() override;

        /*
        * @brief Sets focus to the window using SetFocus and SetForegroundWindow APIs.
        */
        Core::Result<void, Core::WindowError>  Focus() override;

        /*
        * @brief Processes pending Win32 messages using PeekMessage and DispatchMessage.
        */
        Core::Result<void, Core::WindowError>  ProcessEvents() override;

        /*
        * @brief Swaps buffers for rendering (placeholder for OpenGL SwapBuffers).
        */
        Core::Result<void, Core::WindowError>  SwapBuffers() override;

        /*
        * @brief Retrieves the native HWND handle.
        *
        * @return void* The HWND pointer.
        */
        void* GetNativeHandle(NativeHandleType type) const override;

        /*
        * @brief Gets the current window configuration.
        *
        * @return WindowConfig The stored configuration.
        */
        WindowConfig GetConfig() const override;

        /*
        * @brief Checks if the window is open.
        *
        * @return NuBool True if the window is open and initialized.
        */
        NuBool IsOpen() const override;

        /*
        * @brief Checks if the window is currently focused.
        *
        * @return NuBool True if the window has focus.
        */
        Core::NuBool IsFocused() const override;

        /*
        * @brief Sets the window title using SetWindowText API.
        *
        * @param title The new title string.
        */
        Core::Result<void, Core::WindowError>  SetTitle(const std::string& title) override;

        /*
        * @brief Sets the window size using SetWindowPos API.
        *
        * @param width The new width in pixels.
        * @param height The new height in pixels.
        */
        Core::Result<void, Core::WindowError>  SetSize(NuInt32 width, NuInt32 height) override;

        /*
        * @brief Sets the window position using SetWindowPos API.
        *
        * @param x The x-coordinate.
        * @param y The y-coordinate.
        */
       Core::Result<void, Core::WindowError>  SetPosition(NuInt32 x, NuInt32 y) override;

        /*
        * @brief Sets VSync state (stored in config; actual implementation depends on graphics API).
        *
        * @param vsync True to enable VSync.
        */
        Core::Result<void, Core::WindowError>  SetVSync(NuBool vsync) override;

        /*
        * @brief Retrieves the event system for handling window events.
        *
        * @return WindowEventSystem& Reference to the internal event system.
        */
        WindowEventSystem& GetEventSystem() override { return m_EventSystem; }

    private:
        /*
        * @brief Static window procedure callback for Win32 messages.
        *
        * Dispatches messages to the instance's HandleMessage method.
        *
        * @param hwnd The window handle.
        * @param msg The message ID.
        * @param wParam The wParam parameter.
        * @param lParam The lParam parameter.
        * @return LRESULT The result of the message handling.
        */
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        /*
        * @brief Handles Win32 window messages and dispatches events.
        *
        * Processes messages like WM_CLOSE, WM_KEYDOWN, WM_MOUSEMOVE, etc., and pushes
        * corresponding WindowEvent instances to the event system.
        *
        * @param msg The message ID.
        * @param wParam The wParam parameter.
        * @param lParam The lParam parameter.
        * @return LRESULT The result of the message handling.
        */
        LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

        /*
        * @brief Pushes a window event to the event system with a timestamp.
        *
        * @param event The unique pointer to the event.
        */
        Core::Result<void, Core::WindowError>  PushWindowEvent(std::unique_ptr<WindowEvent> event);

        HWND m_HWND = nullptr;              //!< Native Win32 window handle.
        HDC m_HDC = nullptr;                //!< Device context for rendering.
        HINSTANCE m_HInstance = nullptr;    //!< Application instance handle.
        WindowConfig m_Config;              //!< Current window configuration.
        WindowEventSystem m_EventSystem;    //!< System for handling window events.

        NuBool m_IsOpen = false;            //!< Flag indicating if the window is open.
        NuBool m_IsFocused = false;         //!< Flag indicating if the window is focused.
        NuBool m_IsInitialized = false;     //!< Flag indicating if the window is initialized.
    };
}