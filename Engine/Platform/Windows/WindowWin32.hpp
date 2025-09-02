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

	class WindowWin32 final : public IWindow
	{
	public:
		WindowWin32();
		~WindowWin32() override;

		Result<void, Core::WindowError> Initialize(const WindowConfig& config) override;
		void Shutdown() override;

		void Show() override;
		void Hide() override;
		void Focus() override;

		void ProcessEvents() override;
		void SwapBuffers() override;

		void* GetNativeHandle() const override;
		WindowConfig GetConfig() const override;
		NuBool IsOpen() const override;
		NuBool IsFocused() const override;

		void SetTitle(const std::string& title) override;
		void SetSize(NuInt32 width, NuInt32 height) override;
		void SetPosition(NuInt32 x, NuInt32 y) override;
		void SetVSync(NuBool vsync) override;

		WindowEventSystem& GetEventSystem() override { return m_EventSystem; }

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

		void PushWindowEvent(std::unique_ptr<WindowEvent> event);

		HWND m_HWND = nullptr;
		HDC m_HDC = nullptr;
		HINSTANCE m_HInstance = nullptr;
		WindowConfig m_Config;

		WindowEventSystem m_EventSystem;

		NuBool m_IsOpen = false;
		NuBool m_IsFocused = false;
		NuBool m_IsInitialized = false;
	};
}