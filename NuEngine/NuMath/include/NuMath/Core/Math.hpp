// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/Constants.hpp>

#include <cmath>

namespace NuMath
{
	[[nodiscard]] NU_FORCEINLINE float Sin(float x) noexcept { return std::sinf(x); }
	[[nodiscard]] NU_FORCEINLINE float Cos(float x) noexcept { return std::cosf(x); }
	[[nodiscard]] NU_FORCEINLINE float Tan(float x) noexcept { return std::tanf(x); }
	[[nodiscard]] NU_FORCEINLINE float Sqrt(float x) noexcept { return std::sqrtf(x); }
	[[nodiscard]] NU_FORCEINLINE float Abs(float x) noexcept { return std::fabsf(x); }

	[[nodiscard]] NU_FORCEINLINE float ASin(float x) noexcept { return std::asinf(x); }
	[[nodiscard]] NU_FORCEINLINE float ACos(float x) noexcept { return std::acosf(x); }
	[[nodiscard]] NU_FORCEINLINE float ATan(float x) noexcept { return std::atanf(x); }

	template <typename T>
	[[nodiscard]] NU_FORCEINLINE T Min(T a, T b) noexcept { return (a < b) ? a : b; }

	template <typename T>
	[[nodiscard]] NU_FORCEINLINE T Max(T a, T b) noexcept { return (a > b) ? a : b; }

	template <typename T>
	[[nodiscard]] NU_FORCEINLINE T Clamp(T value, T min, T max) noexcept 
	{ 
		return (value < min) ? min : ((value > max) ? max : value); 
	}

	[[nodiscard]] NU_FORCEINLINE float Saturate(float value) noexcept
	{
		return Clamp(value, 0.0f, 1.0f);
	}
}