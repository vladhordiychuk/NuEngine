// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Detail/SIMD/SIMDBackend.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/Math.hpp>

namespace NuMath::MatrixAPI
{
	using Impl = NuMath::Simd::MathBackend;

	// =============================================
	// Type definitions
	// =============================================

	/**
	 *
	 */
	using NuMat4 = Impl::NuMat4;

	/**
	 *
	 */
	using NuMat3 = Impl::NuMat3;

	/**
	 *
	 */
	using NuMat2 = Impl::NuMat2;

	/**
	 *
	 */
	using NuVec4 = Impl::NuVec4;

	// =============================================
	// Setters
	// =============================================

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 SetIdentityMatrix() noexcept
	{
		return Impl::SetIdentityMatrix();
	}

	/**
	 *
	 */
	NU_FORCEINLINE void SetColumn(NuMat4& m, int col, NuVec4 v) noexcept {
		Impl::SetColumn(m, col, v);
	}

	/**
	 *
	 */
	NU_FORCEINLINE void SetRow(NuMat4& m, int row, NuVec4 v) noexcept {
		Impl::SetRow(m, row, v);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 Transpose(NuMat4 mat) noexcept
	{
		return Impl::Transpose(mat);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 Add(NuMat4 a, NuMat4 b) noexcept
	{
		return Impl::Add(a, b);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 Sub(NuMat4 a, NuMat4 b) noexcept
	{
		return Impl::Sub(a, b);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 Mul(NuMat4 a, NuMat4 b) noexcept
	{
		return Impl::Mul(a, b);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(NuMat4 a, NuVec4 b) noexcept
	{
		return Impl::Mul(a, b);
	}
	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 FromRows(NuVec4 r0, NuVec4 r1, NuVec4 r2, NuVec4 r3) noexcept
	{
		return Impl::FromRows(r0, r1, r2, r3);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 FromColumns(NuVec4 c0, NuVec4 c1, NuVec4 c2, NuVec4 c3) noexcept
	{
		return Impl::FromColumns(c0, c1, c2, c3);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE float Determinant(NuMat4 m) noexcept
	{
		return Impl::Determinant(m);
	}


	/**
	 * 
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 Inverse(NuMat4 m) noexcept
	{
		return Impl::Inverse(m);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateTranslation(NuVec4 v) noexcept
	{
		return Impl::CreateTranslation(v);
	}

	/**
	 * 
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateScale(NuVec4 scale) noexcept
	{
		return Impl::CreateScale(scale);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateRotation(NuVec4 quat) noexcept
	{
		return Impl::CreateRotation(quat);
	}

	/**
     *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateLookAt(NuVec4 eye, NuVec4 target, NuVec4 up) noexcept
	{
		return Impl::CreateLookAt(eye, target, up);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreatePerspective(float fovY, float aspect, float nearZ, float farZ) noexcept
	{
		return Impl::CreatePerspective(fovY, aspect, nearZ, farZ);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateOrthographic(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept
	{
		return Impl::CreateOrthographic(left, right, bottom, top, nearZ, farZ);
	}

	/**
     *
	 */
	//using Impl::Decompose; 

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE bool IsIdentity(NuMat4 m, float epsilon = 0.0f) noexcept
	{
		return Impl::IsIdentity(m, epsilon);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE bool Equal(NuMat4 a, NuMat4 b) noexcept
	{
		return Impl::Equal(a, b);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE bool NearEqual(NuMat4 a, NuMat4 b, float epsilon) noexcept
	{
		return Impl::NearEqual(a, b, epsilon);
	}

	/**
	 * 
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(NuMat4 m, int index) noexcept
	{
		return Impl::GetColumn(m, index);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(NuMat4 m, int index) noexcept
	{
		return Impl::GetRow(m, index);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE float Access(const NuMat4& m, int row, int col) noexcept
	{
		return Impl::Access(m, row, col);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE float& Access(NuMat4& m, int row, int col) noexcept
	{
		return Impl::Access(m, row, col);
	}

	/**
	 *
	 */
	[[nodiscard]] NU_FORCEINLINE const float* Data(const NuMat4& m) noexcept
	{
		return Impl::Data(m);
	}
} // namespace NuMath::MatrixAPI