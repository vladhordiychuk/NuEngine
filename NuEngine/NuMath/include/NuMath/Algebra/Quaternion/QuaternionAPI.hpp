// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Detail/SIMD/SIMDBackend.hpp>

namespace NuMath::QuaternionAPI
{
	using Impl = Simd::MathBackend;

	using NuQuat = Simd::NuVec4;
} // namespace NuMath::QuaternionAPI