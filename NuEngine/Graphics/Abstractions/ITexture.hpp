// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>

namespace NuEngine::Graphics
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual const std::string& GetPath() const = 0;
	};
}