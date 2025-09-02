// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Types.hpp>

#include <string>

namespace NuEngine::Platform
{
	using NuInt32 = NuEngine::Core::Types::NuInt32;
	using NuBool = NuEngine::Core::Types::NuBool;

	/*
	* Window configuration structure.
	*/
	struct WindowConfig
	{
		NuInt32 width_ = 1280;
		NuInt32 height_ = 720;
		std::string title_ = "NuEngine Application";

		NuBool resizable_ = true;
		NuBool decorated_ = true;
		NuBool fullscreen_ = false;
		NuBool vsync_ = false;

		NuInt32 x_ = 0;
		NuInt32 y_ = 0;

		WindowConfig() = default;

		WindowConfig(NuInt32 width, NuInt32 height, std::string_view title)
			: width_(width), height_(height), title_(title)
		{
		}

		// Getter methods
		NuInt32 GetWidth() const noexcept { return width_; }
		NuInt32 GetHeight() const noexcept { return height_; }
		const std::string& GetTitle() const noexcept { return title_; }
		NuBool IsResizable() const noexcept { return resizable_; }
		NuBool IsDecorated() const noexcept { return decorated_; }
		NuBool IsFullscreen() const noexcept { return fullscreen_; }
		NuBool IsVSync() const noexcept { return vsync_; }
		NuInt32 GetX() const noexcept { return x_; }
		NuInt32 GetY() const noexcept { return y_; }

		// Setter methods
		WindowConfig& SetSize(NuInt32 width, NuInt32 height) noexcept
		{
			width_ = width;
			height_ = height;
			return *this;
		}

		WindowConfig& SetTitle(std::string_view title) noexcept
		{
			title_ = title;
			return *this;
		}

		WindowConfig& SetFullscreen(NuBool fullscreen) noexcept
		{
			fullscreen_ = fullscreen;
			return *this;
		}

		WindowConfig& SetPosition(NuInt32 x, NuInt32 y) noexcept
		{
			x_ = x;
			y_ = y;
			return *this;
		}

		WindowConfig& SetVSync(NuBool vsync) noexcept
		{
			vsync_ = vsync;
			return *this;
		}

		WindowConfig& SetResizable(NuBool resizable) noexcept
		{
			resizable_ = resizable;
			return *this;
		}

		WindowConfig& SetDecorated(NuBool decorated) noexcept
		{
			decorated_ = decorated;
			return *this;
		}

		// Validation methods
		NuBool IsValid() const noexcept
		{
			return width_ > 0 && height_ > 0 && !title_.empty();
		}

		// Utility methods
		float GetAspectRatio() const noexcept
		{
			return height_ > 0 ? static_cast<float>(width_) / height_ : 0.0f;
		}
	};
}