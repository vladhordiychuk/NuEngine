// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Detail/SIMD/SIMDBackend.hpp>
#include <NuMath/Algebra/Vector/VectorAPI.hpp>

namespace NuMath::QuaternionAPI
{
	using Impl = Simd::MathBackend;

	using NuQuat = Impl::NuVec4;
	using NuVec4 = Impl::NuVec4;

	// =============================================
	// Basic operations (reuse from VectorAPI)
	// =============================================

	using VectorAPI::Load;
	using VectorAPI::Store;
	using VectorAPI::Set;
	using VectorAPI::SetAll;
	using VectorAPI::Add;
	using VectorAPI::Sub;
	using VectorAPI::Div;
	using VectorAPI::Lerp;
	using VectorAPI::Min;
	using VectorAPI::Max;
	using VectorAPI::GetX;
	using VectorAPI::GetY;
	using VectorAPI::GetZ;
	using VectorAPI::GetW;
	using VectorAPI::SetX;
	using VectorAPI::SetY;
	using VectorAPI::SetZ;
	using VectorAPI::SetW;
	using VectorAPI::Equal;
	using VectorAPI::NearEqual;
	using VectorAPI::Length4;
	using VectorAPI::Normalize4;
	using VectorAPI::Dot4;

	// =============================================
	// Quaternion-specific operations
	// =============================================

	[[nodiscard]] static NU_FORCEINLINE NuQuat Identity() noexcept
	{
		return Impl::QuatIdentity();
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat Conjugate(NuQuat q) noexcept
	{
		return Impl::QuatConjugate(q);
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat Inverse(NuQuat q) noexcept
	{
		return Impl::QuatInverse(q);
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat Mul(NuQuat a, NuQuat b) noexcept
	{
		return Impl::QuatMul(a, b);
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat Mul(NuQuat a, float scalar) noexcept
	{
		return Impl::Mul(a, Impl::SetAll(scalar));
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat RotateVector(NuQuat q, NuVec4 v) noexcept
	{
		return Impl::QuatRotateVector(q, v);
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat FromAxisAngle(NuQuat axis, float angleRadians) noexcept
	{
		return Impl::QuatFromAxisAngle(axis, angleRadians);
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat FromEuler(float pitch, float yall, float roll) noexcept
	{
		return Impl::QuatFromEuler(pitch, yall, roll);
	}

	[[nodiscard]] static NU_FORCEINLINE NuQuat Slerp(NuQuat a, NuQuat b, float t) noexcept
	{
		return Impl::Slerp(a, b, t);
	}

} // namespace NuMath::QuaternionAPI