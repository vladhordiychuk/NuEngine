// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <immintrin.h>

#include <Core/Types/Types.hpp>
#include <Math/StorageTypes.hpp>

namespace NuEngine::Math::AVX
{
	using NuFloat = Core::Types::NuFloat;
	using NuBool = Core::Types::NuBool;
	using NuInt32 = Core::Types::NuInt32;

	// =============================================
	// Common types
	// =============================================

	/*
	* @brief 8-component vector type with 32-byte alignment.
	* 
	* Represents a SIMD-friendly vector with four float components (x, y, z, w, ).
	*/
	using NuVec8 = __m256;

	[[nodiscard]] NU_FORCEINLINE NuVec8 Load(NuVecStorage8 vec) noexcept
	{
		return _mm256_loadu_ps(vec.f);
	}

	[[nodiscard]] NU_FORCEINLINE void Store(NuVecStorage8 vec, NuVec8 vec) noexcept
	{
		_mm256_storeu_ps(vec.f, vec);
	}
	
	[[nodiscard]] NU_FORCEINLINE NuVec8 Set(NuFloat e0, NuFloat e1, NuFloat e2, NuFloat e3, NuFloat e4, NuFloat e5, NuFloat e6, NuFloat e7) noexcept
	{
		return _mm256_setr_ps(e0, e1, e2, e3, e4, e5, e6, e7);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 SetAll(NuFloat value) noexcept
	{
		return _mm256_set1_ps(value);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 SetZero() noexcept
	{
		return _mm256_setzero_ps();
	}

	template <NuInt32 index>
	[[nodiscard]] NU_FORCEINLINE NuFloat Get(NuVec8 vec) noexcept
	{
		Core::Types::NuAssert(index >= 0 && index < 8, "Get index out of bounds for NuVec8");
		return vec.m256_f32[index];
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Add(NuVec8 a, NuVec8 b) noexcept
	{
		return _mm256_add_ps(a, b);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Sub(NuVec8 a, NuVec8 b) noexcept
	{
		return _mm256_sub_ps(a, b);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Mul(NuVec8 a, NuVec8 b) noexcept
	{
		return _mm256_mul_ps(a, b);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Div(NuVec8 a, NuVec8 b) noexcept
	{
		Core::Types::NuAssert();
		return _mm256_div_ps(a, b);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Min(NuVec8 a, NuVec8 b) noexcept
	{
		return _mm256_min_ps(a, b);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Max(NuVec8 a, NuVec8 b) noexcept
	{
		return _mm256_max_ps(a, b);
	}

	[[nodiscard]] NU_FORCEINLINE NuVec8 Abs(NuVec8 a, NuVec8 b) noexcept
	{
		NuFloat mask = 

	}
}