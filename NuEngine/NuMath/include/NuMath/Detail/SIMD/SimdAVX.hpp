// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.

#pragma once

#include <immintrin.h>
#include <algorithm>

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/Math.hpp>
#include <NuMath/Core/Constants.hpp>

namespace NuMath::Detail
{
	struct AVX_Traits
	{
		// =============================================
		// Types & Constants
		// =============================================

		using Register = __m256;

		using NuVec8 = __m256;

		static constexpr int Width = 8;

		// =============================================
		// Load / Store
		// =============================================

		[[nodiscard]] static NU_FORCEINLINE Register Load(const float* ptr) noexcept
		{
			return _mm256_load_ps(ptr);
		}

		static NU_FORCEINLINE void Store(float* ptr, Register val) noexcept
		{
			_mm256_store_ps(ptr, val);
		}

		static NU_FORCEINLINE void Stream(float* ptr, Register val) noexcept
		{
			_mm256_stream_ps(ptr, val);
		}

		[[nodiscard]] static NU_FORCEINLINE Register SetZero() noexcept
		{
			return _mm256_setzero_ps();
		}

		[[nodiscard]] static NU_FORCEINLINE Register SetAll(float v) noexcept
		{
			return _mm256_set1_ps(v);
		}

		// =============================================
		// Arithmetic
		// =============================================

		[[nodiscard]] static NU_FORCEINLINE Register Add(Register a, Register b) noexcept
		{
			return _mm256_add_ps(a, b);
		}

		[[nodiscard]] static NU_FORCEINLINE Register Sub(Register a, Register b) noexcept
		{
			return _mm256_sub_ps(a, b);
		}

		[[nodiscard]] static NU_FORCEINLINE Register Mul(Register a, Register b) noexcept
		{
			return _mm256_mul_ps(a, b);
		}

		[[nodiscard]] static NU_FORCEINLINE Register Div(Register a, Register b) noexcept
		{
			return _mm256_div_ps(a, b);
		}

		[[nodiscard]] static NU_FORCEINLINE Register Neg(Register a) noexcept
		{
			return _mm256_sub_ps(_mm256_setzero_ps(), a);
		}
	};
}