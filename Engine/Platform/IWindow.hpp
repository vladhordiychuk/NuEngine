// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Types.hpp>
#include <Core/Types/Result.hpp>
#include <Core/Errors/WindowError.hpp>

#include <Platform/Common/WindowConfig.hpp>
#include <Platform/Common/WindowEvents.hpp>

#include <memory>

namespace NuEngine::Platform
{
	using NuInt32 = NuEngine::Core::Types::NuInt32;
	using NuBool = NuEngine::Core::Types::NuBool;

	template<typename T, typename E>
	using Result = NuEngine::Core::Result<T, E>;

	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual Result<void, Core::WindowError> Initialize(const WindowConfig& config) = 0;
		virtual void Shutdown() = 0;

		virtual void Show() = 0;
		virtual void Hide() = 0;
		virtual void Focus() = 0;

		virtual void ProcessEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual void* GetNativeHandle() const = 0;
		virtual WindowConfig GetConfig() const = 0;

		[[nodiscard]] virtual NuBool IsOpen() const = 0;
		[[nodiscard]] virtual NuBool IsFocused() const = 0;

		virtual void SetTitle(const std::string& title) = 0;
		virtual void SetSize(NuInt32 width, NuInt32 height) = 0;
		virtual void SetPosition(NuInt32 x, NuInt32 y) = 0;
		virtual void SetVSync(NuBool vsync) = 0;

		virtual WindowEventSystem& GetEventSystem() = 0;
	};

	using WindowPtr = std::unique_ptr<IWindow>;
	WindowPtr CreatePlatformWindow();
}