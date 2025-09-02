#include <Platform/Windows/WindowWin32.hpp>
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
		Shutdown();
	}

	Result<void, Core::WindowError> WindowWin32::Initialize(const WindowConfig& config)
	{
		m_Config = config;
		m_HInstance = GetModuleHandle(nullptr);

		WNDCLASSEX wc{};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowWin32::WndProc;
		wc.hInstance = m_HInstance;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.lpszClassName = L"NuEngineWindowClass";

		if (!RegisterClassEx(&wc)) {
			return Err(Core::WindowError::PlatformFailure);
		}

		std::wstring wideTitle(m_Config.GetTitle().begin(), m_Config.GetTitle().end());

		DWORD style = WS_OVERLAPPEDWINDOW;
		if (!m_Config.IsResizable()) style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
		if (!m_Config.IsDecorated()) style = WS_POPUP;

		m_HWND = CreateWindowEx(
			0,
			wc.lpszClassName,
			wideTitle.c_str(),
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			m_Config.GetWidth(), m_Config.GetHeight(),
			nullptr, nullptr, m_HInstance, this
		);

		if (!m_HWND) {
			return Err(Core::WindowError::PlatformFailure);
		}

		SetWindowLongPtr(m_HWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		m_HDC = GetDC(m_HWND);
		if (!m_HDC) {
			DestroyWindow(m_HWND);
			m_HWND = nullptr;
			return Err(Core::WindowError::PlatformFailure);
		}

		m_IsOpen = true;
		m_IsInitialized = true;

		ShowWindow(m_HWND, SW_SHOW);
		UpdateWindow(m_HWND);

		return NuEngine::Core::Ok<Core::WindowError>();
	}

	void WindowWin32::Shutdown()
	{
		if (m_HDC && m_HWND) {
			ReleaseDC(m_HWND, m_HDC);
			m_HDC = nullptr;
		}

		if (m_HWND) {
			DestroyWindow(m_HWND);
			m_HWND = nullptr;
		}

		m_IsOpen = false;
		m_IsInitialized = false;
	}

	void WindowWin32::Show()
	{
		if (m_HWND) {
			ShowWindow(m_HWND, SW_SHOW);
			UpdateWindow(m_HWND);
		}
	}

	void WindowWin32::Hide()
	{
		if (m_HWND) {
			ShowWindow(m_HWND, SW_HIDE);
		}
	}

	void WindowWin32::Focus()
	{
		if (m_HWND) {
			SetFocus(m_HWND);
			SetForegroundWindow(m_HWND);
		}
	}

	void WindowWin32::ProcessEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowWin32::SwapBuffers()
	{
		// Для OpenGL: ::SwapBuffers(m_HDC);
	}

	void* WindowWin32::GetNativeHandle() const
	{
		return m_HWND;
	}

	WindowConfig WindowWin32::GetConfig() const
	{
		return m_Config;
	}

	NuBool WindowWin32::IsOpen() const
	{
		return m_IsOpen;
	}

	NuBool WindowWin32::IsFocused() const
	{
		return m_IsFocused;
	}

	void WindowWin32::SetTitle(const std::string& title)
	{
		if (m_HWND) {
			std::wstring wideTitle(title.begin(), title.end());
			SetWindowText(m_HWND, wideTitle.c_str());
			m_Config.SetTitle(title);
		}
	}

	void WindowWin32::SetSize(NuInt32 width, NuInt32 height)
	{
		if (m_HWND) {
			SetWindowPos(m_HWND, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
			m_Config.SetSize(width, height);
		}
	}

	void WindowWin32::SetPosition(NuInt32 x, NuInt32 y)
	{
		if (m_HWND) {
			SetWindowPos(m_HWND, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			m_Config.SetPosition(x, y);
		}
	}

	void WindowWin32::SetVSync(NuBool vsync)
	{
		m_Config.SetVSync(vsync);
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

	void WindowWin32::PushWindowEvent(std::unique_ptr<WindowEvent> event)
	{
		event->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
		m_EventSystem.PushEvent(std::move(event));
	}

	LRESULT WindowWin32::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();

		switch (msg) {
		case WM_CLOSE:
			PushWindowEvent(std::make_unique<WindowClosedEvent>());
			DestroyWindow(m_HWND);
			return 0;

		case WM_DESTROY:
			m_IsOpen = false;
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
		{
			NuInt32 width = LOWORD(lParam);
			NuInt32 height = HIWORD(lParam);
			m_Config.SetSize(width, height);

			auto resizeEvent = std::make_unique<WindowResizedEvent>();
			resizeEvent->width = width;
			resizeEvent->height = height;
			PushWindowEvent(std::move(resizeEvent));
			break;
		}

		case WM_MOVE:
		{
			NuInt32 x = LOWORD(lParam);
			NuInt32 y = HIWORD(lParam);
			m_Config.SetPosition(x, y);

			auto moveEvent = std::make_unique<WindowMovedEvent>();
			moveEvent->x = x;
			moveEvent->y = y;
			PushWindowEvent(std::move(moveEvent));
			break;
		}

		case WM_SETFOCUS:
			m_IsFocused = true;
			{
				auto focusEvent = std::make_unique<WindowFocusEvent>();
				focusEvent->focused = true;
				PushWindowEvent(std::move(focusEvent));
			}
			break;

		case WM_KILLFOCUS:
			m_IsFocused = false;
			{
				auto focusEvent = std::make_unique<WindowFocusEvent>();
				focusEvent->focused = false;
				PushWindowEvent(std::move(focusEvent));
			}
			break; // <- Доданий break

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
			break;
		}

		case WM_MOUSEMOVE:
		{
			auto mouseEvent = std::make_unique<MouseMoveEvent>();
			mouseEvent->x = GET_X_LPARAM(lParam);
			mouseEvent->y = GET_Y_LPARAM(lParam);
			PushWindowEvent(std::move(mouseEvent));
			break;
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

			PushWindowEvent(std::move(buttonEvent));
			break;
		}

		case WM_MOUSEWHEEL:
		{
			auto scrollEvent = std::make_unique<ScrollEvent>();
			scrollEvent->xoffset = 0.0;
			scrollEvent->yoffset = static_cast<double>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
			PushWindowEvent(std::move(scrollEvent));
			break;
		}
		}

		return DefWindowProc(m_HWND, msg, wParam, lParam);
	}
}