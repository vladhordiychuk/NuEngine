// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/Constants.hpp>
#include <NuMath/Core/Math.hpp>

#include <arm_neon.h>

namespace NuMath::Detail
{
	struct NEON_Traits
	{
		using NuVec4 = float32x4_t;

		

		// \copydoc NuMath::VectorAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Add(NuVec4 a, NuVec4 b) noexcept
		{
			return vaddq_f32(a, b);
		}

		// \copydoc NuMath::VectorAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Sub(NuVec4 a, NuVec4 b) noexcept
		{
			return vsubq_f32(a, b);
		}

		// \copydoc NuMath::VectorAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(NuVec4 a, NuVec4 b) noexcept
		{
			return vmulq_f32(a, b);
		}

		// \copydoc NuMath::VectorAPI::Div
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Div(NuVec4 a, NuVec4 b) noexcept
		{

			return vdivq_f32(a, b);
		}

		// \copydoc NuMath::VectorAPI::Min
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Min(NuVec4 a, NuVec4 b) noexcept
		{
			return vminq_f32(a, b);
		}

		// \copydoc NuMath::VectorAPI::Max
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Max(NuVec4 a, NuVec4 b) noexcept
		{
			return vmaxq_f32(a, b);
		}
	};

}