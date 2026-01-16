// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>

namespace NuEngine::Platform
{
	/*
	* Window configuration structure.
	*/
	struct WindowConfig
	{
		int width_ = 1280;
		int height_ = 720;
		std::string title_ = "NuEngine Application";

		bool resizable_ = true;
		bool decorated_ = true;
		bool fullscreen_ = false;
		bool vsync_ = false;

		int x_ = 0;
		int y_ = 0;

		WindowConfig() = default;

		WindowConfig(int width, int height, std::string_view title)
			: width_(width), height_(height), title_(title)
		{
		}

		int GetWidth() const noexcept { return width_; }
		int GetHeight() const noexcept { return height_; }
		const std::string& GetTitle() const noexcept { return title_; }
		bool IsResizable() const noexcept { return resizable_; }
		bool IsDecorated() const noexcept { return decorated_; }
		bool IsFullscreen() const noexcept { return fullscreen_; }
		bool IsVSync() const noexcept { return vsync_; }
		int GetX() const noexcept { return x_; }
		int GetY() const noexcept { return y_; }

		WindowConfig& SetSize(int width, int height) noexcept
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

		WindowConfig& SetFullscreen(bool fullscreen) noexcept
		{
			fullscreen_ = fullscreen;
			return *this;
		}

		WindowConfig& SetPosition(int x, int y) noexcept
		{
			x_ = x;
			y_ = y;
			return *this;
		}

		WindowConfig& SetVSync(bool vsync) noexcept
		{
			vsync_ = vsync;
			return *this;
		}

		WindowConfig& SetResizable(bool resizable) noexcept
		{
			resizable_ = resizable;
			return *this;
		}

		WindowConfig& SetDecorated(bool decorated) noexcept
		{
			decorated_ = decorated;
			return *this;
		}

		bool IsValid() const noexcept
		{
			return width_ > 0 && height_ > 0 && !title_.empty();
		}

		float GetAspectRatio() const noexcept
		{
			return height_ > 0 ? static_cast<float>(width_) / height_ : 0.0f;
		}
	};
}