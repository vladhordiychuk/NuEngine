#include <Platform/Windows/WindowWin32.hpp>
#include <windows.h>
#include <windowsx.h>
#include <chrono>

namespace NuEngine::Platform
{
    WindowWin32::WindowWin32()
        : m_HWND(nullptr)
        , m_HDC(nullptr)
        , m_HInstance(nullptr)
        , m_IsOpen(false)
        , m_IsFocused(false)
        , m_IsInitialized(false)
    {
    }

    WindowWin32::~WindowWin32()
    {
        auto result = Shutdown();
        if (result.IsError()) {
            
        }
    }

    Core::Result<void, WindowError> WindowWin32::Initialize(const WindowConfig& config)
    {
        if (m_IsInitialized) 
        {
            return Core::Err(WindowError(WindowErrorCode::AlreadyInitialized));
        }

        m_Config = config;
        m_HInstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc{};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WindowWin32::WndProc;
        wc.hInstance = m_HInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"NuEngineWindowClass";

        if (!RegisterClassEx(&wc)) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }

        int len = MultiByteToWideChar(CP_UTF8, 0, m_Config.GetTitle().c_str(), -1, nullptr, 0);
        if (len == 0) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        std::wstring wideTitle(len, 0);
        MultiByteToWideChar(CP_UTF8, 0, m_Config.GetTitle().c_str(), -1, &wideTitle[0], len);

        DWORD style = WS_OVERLAPPEDWINDOW;
        if (!m_Config.IsResizable()) style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
        if (!m_Config.IsDecorated()) style = WS_POPUP;

        RECT rect{ 0, 0, static_cast<LONG>(m_Config.GetWidth()), static_cast<LONG>(m_Config.GetHeight()) };
        AdjustWindowRect(&rect, style, FALSE);
        int totalWidth = rect.right - rect.left;
        int totalHeight = rect.bottom - rect.top;

        m_HWND = CreateWindowEx(
            0,
            wc.lpszClassName,
            wideTitle.c_str(),
            style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            totalWidth, totalHeight,
            nullptr, nullptr, m_HInstance, this
        );

        if (!m_HWND) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }

        SetWindowLongPtr(m_HWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        m_HDC = GetDC(m_HWND);
        if (!m_HDC) 
        {
            DestroyWindow(m_HWND);
            m_HWND = nullptr;
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }

        m_IsOpen = true;
        m_IsInitialized = true;
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::Shutdown()
    {
        if (!m_IsInitialized)
        {
            return Core::Ok();
        }

        if (m_HDC && m_HWND) 
        {
            int released = ReleaseDC(m_HWND, m_HDC);
            m_HDC = nullptr;
            if (released == 0) 
            {
                return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
            }
        }

        if (m_HWND) 
        {
            BOOL destroyed = DestroyWindow(m_HWND);
            m_HWND = nullptr;
            if (!destroyed) 
            {
                return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
            }
        }

        m_IsOpen = false;
        m_IsInitialized = false;
        return Core::Ok();
    }


    Core::Result<void, WindowError> WindowWin32::Show()
    {
        if (!m_HWND) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }

        ShowWindow(m_HWND, SW_SHOW);
        if (!UpdateWindow(m_HWND)) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }

        return Core::Ok();
    }


    Core::Result<void, WindowError> WindowWin32::Hide()
    {
        if (!m_HWND) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        if (!ShowWindow(m_HWND, SW_HIDE)) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::Focus()
    {
        if (!m_HWND) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        SetFocus(m_HWND);
        SetForegroundWindow(m_HWND);
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::ProcessEvents()
    {
        MSG msg;
        while (PeekMessage(&msg, m_HWND, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT)
            {
                m_IsOpen = false;
                return Core::Ok();
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::SwapBuffers()
    {
        if (!m_HDC) 
        {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        if (!::SwapBuffers(m_HDC)) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        return Core::Ok();
    }

    void* WindowWin32::GetNativeHandle(NativeHandleType type) const
    {
        switch (type)
        {
        case NativeHandleType::Window:
            return m_HWND; 
        case NativeHandleType::Display:
            return m_HDC;
        default:
            return nullptr;
        }
    }

    WindowConfig WindowWin32::GetConfig() const
    {
        return m_Config;
    }

    bool WindowWin32::IsOpen() const
    {
        return m_IsOpen;
    }

    bool WindowWin32::IsFocused() const
    {
        return m_IsFocused;
    }

    Core::Result<void, WindowError> WindowWin32::SetTitle(const std::string& title)
    {
        if (!m_HWND) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        int len = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
        std::wstring wideTitle(len, 0);
        MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], len);
        if (!SetWindowText(m_HWND, wideTitle.c_str())) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        m_Config.SetTitle(title);
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::SetSize(int width, int height)
    {
        if (!m_HWND) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        RECT rect{ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
        DWORD style = GetWindowLong(m_HWND, GWL_STYLE);
        AdjustWindowRect(&rect, style, FALSE);
        if (!SetWindowPos(m_HWND, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER)) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        m_Config.SetSize(width, height);
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::SetPosition(int x, int y)
    {
        if (!m_HWND) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        RECT rect{ 0, 0, 0, 0 };
        DWORD style = GetWindowLong(m_HWND, GWL_STYLE);
        AdjustWindowRect(&rect, style, FALSE);
        if (!SetWindowPos(m_HWND, nullptr, x - rect.left, y - rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER)) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        m_Config.SetPosition(x, y);
        return Core::Ok();
    }

    Core::Result<void, WindowError> WindowWin32::SetVSync(bool vsync)
    {
        m_Config.SetVSync(vsync);
        return Core::Ok();
    }

    LRESULT CALLBACK WindowWin32::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_CREATE) {
            LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
            return 0;
        }

        WindowWin32* window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (window) {
            return window->HandleMessage(msg, wParam, lParam);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    Core::Result<void, WindowError> WindowWin32::PushWindowEvent(std::unique_ptr<WindowEvent> event)
    {
        if (!event) {
            return Core::Err(WindowError(WindowErrorCode::PlatformFailure));
        }
        event->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        m_EventSystem.PushEvent(std::move(event));
        return Core::Ok();
    }

    LRESULT WindowWin32::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg) 
        {
            case WM_CLOSE:
            {
                auto result = PushWindowEvent(std::make_unique<WindowClosedEvent>());
                if (result.IsError()) {
                    return 0;
                }
                m_IsOpen = false;
                return 0;
            }
            case WM_DESTROY:
                m_IsOpen = false;
                return 0;
            case WM_SIZE:
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                m_Config.SetSize(width, height);
                auto resizeEvent = std::make_unique<WindowResizedEvent>();
                resizeEvent->width = width;
                resizeEvent->height = height;
                PushWindowEvent(std::move(resizeEvent));
                return 0;
            }
            case WM_MOVE:
            {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                m_Config.SetPosition(x, y);
                auto moveEvent = std::make_unique<WindowMovedEvent>();
                moveEvent->x = x;
                moveEvent->y = y;
                PushWindowEvent(std::move(moveEvent));
                return 0;
            }
            case WM_SETFOCUS:
            {
                m_IsFocused = true;
                auto focusEvent = std::make_unique<WindowFocusEvent>();
                focusEvent->focused = true;
                PushWindowEvent(std::move(focusEvent));
                return 0;
            }
            case WM_KILLFOCUS:
            {
                m_IsFocused = false;
                auto focusEvent = std::make_unique<WindowFocusEvent>();
                focusEvent->focused = false;
                PushWindowEvent(std::move(focusEvent));
                return 0;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            {
                auto keyEvent = std::make_unique<KeyEvent>();
                keyEvent->key = static_cast<int>(wParam);
                keyEvent->scancode = (lParam >> 16) & 0xFF;
                keyEvent->action = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) ? 1 : 0;
                keyEvent->mods = 0;
                if (GetKeyState(VK_SHIFT) & 0x8000) keyEvent->mods |= 0x01;
                if (GetKeyState(VK_CONTROL) & 0x8000) keyEvent->mods |= 0x02;
                if (GetKeyState(VK_MENU) & 0x8000) keyEvent->mods |= 0x04;
                PushWindowEvent(std::move(keyEvent));
                return 0;
            }
            case WM_MOUSEMOVE:
            {
                auto mouseEvent = std::make_unique<MouseMoveEvent>();
                mouseEvent->x = GET_X_LPARAM(lParam);
                mouseEvent->y = GET_Y_LPARAM(lParam);
                PushWindowEvent(std::move(mouseEvent));
                return 0;
            }
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            {
                auto buttonEvent = std::make_unique<MouseButtonEvent>();
                buttonEvent->button = (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) ? 0 :
                    (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) ? 1 : 2;
                buttonEvent->action = (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN) ? 1 : 0;
                buttonEvent->x = GET_X_LPARAM(lParam);
                buttonEvent->y = GET_Y_LPARAM(lParam);
                buttonEvent->mods = 0;
                if (wParam & MK_SHIFT) buttonEvent->mods |= 0x01;
                if (wParam & MK_CONTROL) buttonEvent->mods |= 0x02;
                if (GetAsyncKeyState(VK_MENU) & 0x8000) buttonEvent->mods |= 0x04;
                PushWindowEvent(std::move(buttonEvent));
                return 0;
            }
            case WM_MOUSEWHEEL:
            {
                auto scrollEvent = std::make_unique<ScrollEvent>();
                scrollEvent->xoffset = 0.0;
                scrollEvent->yoffset = static_cast<double>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
                PushWindowEvent(std::move(scrollEvent));
                return 0;
            }
            case WM_DPICHANGED:
            {
                auto* suggestedRect = reinterpret_cast<RECT*>(lParam);
                SetWindowPos(m_HWND,
                    nullptr,
                    suggestedRect->left,
                    suggestedRect->top,
                    suggestedRect->right - suggestedRect->left,
                    suggestedRect->bottom - suggestedRect->top,
                    SWP_NOZORDER | SWP_NOACTIVATE);
                return 0;
            }
        }
        return DefWindowProc(m_HWND, msg, wParam, lParam);
    }
}