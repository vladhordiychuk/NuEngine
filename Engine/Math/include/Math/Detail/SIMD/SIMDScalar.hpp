// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cmath>
#include <cassert>
#include <bit>

#include <Core/Types/Types.hpp>

namespace NuEngine::Math::Simd_Scalar
{
	using NuInt32 = NuEngine::Core::Types::NuInt32;
	using NuFloat = NuEngine::Core::Types::NuFloat;
	using NuBool = NuEngine::Core::Types::NuBool;

	// =============================================
	// Common types
	// =============================================

	/*
	* @brief 4-component vector type with 16-byte alignment.
	*
	* Represents a SIMD-friendly vector with four float components (x, y, z, w).
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuVec4
	{
		NuFloat x, y, z, w;
	};

	/*
	* @brief 4x4 matrix type with 16-byte alignment.
	*
	* Represents a row-major 4x4 matrix stored in a 2D array.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat4
	{
		NuFloat mat[4][4];
	};
	
	/*
	* @brief 3x3 matrix type with 16-byte alignment.
	*
	* Represents a row-major 3x3 matrix stored in a 2D array.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat3
	{
		NuFloat mat[3][3];
	};

	/*
	* @brief 2x2 matrix type with 16-byte alignment.
	*
	* Represents a row-major 2x2 matrix stored in a 2D array.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat2
	{
		NuFloat mat[2][2];
	};

	// =============================================
	// Vectors
	// =============================================

	// \copydoc NuEngine::Math::VectorAPI::Set
	[[nodiscard]] NU_FORCEINLINE NuVec4 Set(NuFloat x, NuFloat y, NuFloat z, NuFloat w = 0.0f) noexcept
	{
		return { x, y, z, w };
	}

	// \copydoc NuEngine::Math::VectorAPI::SetAll
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetAll(NuFloat scalar) noexcept
	{
		return { scalar, scalar, scalar, scalar };
	}

	// \copydoc NuEngine::Math::VectorAPI::SetZero
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetZero() noexcept
	{
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// \copydoc NuEngine::Math::VectorAPI::GetX
	[[nodiscard]] NU_FORCEINLINE NuFloat GetX(const NuVec4& v) noexcept
	{
		return v.x;
	}

	// \copydoc NuEngine::Math::VectorAPI::GetY
	[[nodiscard]] NU_FORCEINLINE NuFloat GetY(const NuVec4& v) noexcept
	{
		return v.y;
	}

	// \copydoc NuEngine::Math::VectorAPI::GetZ
	[[nodiscard]] NU_FORCEINLINE NuFloat GetZ(const  NuVec4& v) noexcept
	{
		return v.z;
	}

	// \copydoc NuEngine::Math::VectorAPI::GetW
	[[nodiscard]] NU_FORCEINLINE NuFloat GetW(const NuVec4& v) noexcept
	{
		return v.w;
	}

	// \copydoc NuEngine::Math::VectorAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuVec4 Add(const NuVec4& a, const NuVec4& b) noexcept
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuVec4 Sub(const NuVec4& a, const NuVec4& b) noexcept
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuVec4& a, const NuVec4& b) noexcept
	{ 
		return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Div
	[[nodiscard]] NU_FORCEINLINE NuVec4 Div(const NuVec4& a, const NuVec4& b) noexcept
	{
		return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Neg
	[[nodiscard]] NU_FORCEINLINE NuVec4 Neg(const NuVec4& a) noexcept
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Min
	[[nodiscard]] NU_FORCEINLINE NuVec4 Min(const NuVec4& a, const NuVec4& b) noexcept
	{
		return {
			(a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y,
			(a.z < b.z) ? a.z : b.z,
			(a.w < b.w) ? a.w : b.w
		};
	}

	// \copydoc NuEngine::Math::VectorAPI::Max
	[[nodiscard]] NU_FORCEINLINE NuVec4 Max(const NuVec4& a, const NuVec4& b) noexcept
	{
		return {
			(a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y,
			(a.z > b.z) ? a.z : b.z,
			(a.w > b.w) ? a.w : b.w
		};
	}

	// \copydoc NuEngine::Math::VectorAPI::Abs
	[[nodiscard]] NU_FORCEINLINE NuVec4 Abs(const NuVec4& v) noexcept
	{
		return { std::fabs(v.x), std::fabs(v.y), std::fabs(v.z), std::fabs(v.w) };
	}

	// \copydoc NuEngine::Math::VectorAPI::Equal
	[[nodiscard]] NU_FORCEINLINE NuBool Equal(const NuVec4& a, const NuVec4& b) noexcept
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
	}

	// \copydoc NuEngine::Math::VectorAPI::NearEqual
	[[nodiscard]] NU_FORCEINLINE NuBool NearEqual(const NuVec4& a, const NuVec4& b, NuFloat epsilon) noexcept
	{
		return (std::fabs(a.x - b.x) <= epsilon) &&
			   (std::fabs(a.y - b.y) <= epsilon) &&
			   (std::fabs(a.z - b.z) <= epsilon) &&
			   (std::fabs(a.w - b.w) <= epsilon);
	}

	// \copydoc NuEngine::Math::VectorAPI::SqrtScalar
	[[nodiscard]] NU_FORCEINLINE NuFloat SqrtScalar(NuFloat scalar) noexcept
	{ 
		return std::sqrt(scalar);
	}

	// \copydoc NuEngine::Math::VectorAPI::HorizontalAdd4
	[[nodiscard]] NU_FORCEINLINE NuFloat HorizontalAdd4(const NuVec4& v) noexcept
	{
		return v.x + v.y + v.z + v.w;
	}

	// \copydoc NuEngine::Math::VectorAPI::HorizontalAdd3
	[[nodiscard]] NU_FORCEINLINE NuFloat HorizontalAdd3(const NuVec4& v) noexcept
	{
		return v.x + v.y + v.z;
	}

	// \copydoc NuEngine::Math::VectorAPI::HorizontalAdd2
	[[nodiscard]] NU_FORCEINLINE NuFloat HorizontalAdd2(const NuVec4& v) noexcept
	{
		return v.x + v.y;
	}

	[[nodiscard]] NU_FORCEINLINE NuFloat InvSqrtFast(NuFloat x) noexcept 
	{
		const NuFloat xhalf = 0.5f * x;
		int i = std::bit_cast<NuInt32>(x);
		i = 0x5f3759df - (i >> 1);
		NuFloat result = std::bit_cast<NuFloat>(i);
		result = result * (1.5f - xhalf * result * result);
		return result;
	}

	// \copydoc NuEngine::Math::VectorAPI::Normalize2
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize2(const NuVec4& v) noexcept
	{
		NuFloat lenSq = v.x * v.x + v.y * v.y;
		NuEngine::Core::Types::NuAssert(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
		NuFloat invLen = InvSqrtFast(lenSq);
		return { v.x * invLen, v.y * invLen, v.z, v.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Normalize3
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize3(const NuVec4& v) noexcept
	{
		const NuFloat lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
		NuEngine::Core::Types::NuAssert(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
		const NuFloat invLen = InvSqrtFast(lenSq);
		return { v.x * invLen, v.y * invLen, v.z * invLen, v.w };
	}

	// \copydoc NuEngine::Math::VectorAPI::Normalize4
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize4(const NuVec4& v) noexcept
	{
		NuFloat lenSq = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
		NuEngine::Core::Types::NuAssert(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
		NuFloat invLength = InvSqrtFast(lenSq);
		return { v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength };
	}

	// \copydoc NuEngine::Math::VectorAPI::Cross
	[[nodiscard]] NU_FORCEINLINE NuVec4 Cross(const NuVec4& a, const NuVec4& b) noexcept
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x,
			0.0f
		};
	}

	// \copydoc NuEngine::Math::VectorAPI::Dot2
	[[nodiscard]] NU_FORCEINLINE NuFloat Dot2(const NuVec4& a, const NuVec4& b) noexcept
	{
		return a.x * b.x + a.y * b.y;
	}

	// \copydoc NuEngine::Math::VectorAPI::Dot3
	[[nodiscard]] NU_FORCEINLINE NuFloat Dot3(const NuVec4& a, const NuVec4& b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	// \copydoc NuEngine::Math::VectorAPI::Dot4
	[[nodiscard]] NU_FORCEINLINE NuFloat Dot4(const NuVec4& a, const NuVec4& b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	// \copydoc NuEngine::Math::VectorAPI::Length2
	[[nodiscard]] NU_FORCEINLINE NuFloat Length2(const NuVec4& v) noexcept
	{
		return SqrtScalar(Dot2(v, v));
	}

	// \copydoc NuEngine::Math::VectorAPI::Length3
	[[nodiscard]] NU_FORCEINLINE NuFloat Length3(const NuVec4& v) noexcept
	{
		return SqrtScalar(Dot3(v, v));
	}

	// \copydoc NuEngine::Math::VectorAPI::Length4
	[[nodiscard]] NU_FORCEINLINE NuFloat Length4(const NuVec4& v) noexcept
	{
		return SqrtScalar(Dot4(v, v));
	}

	// \copydoc NuEngine::Math::VectorAPI::Lerp
	[[nodiscard]] NU_FORCEINLINE NuVec4 Lerp(const NuVec4& a, const NuVec4& b, NuFloat t) noexcept
	{
		return {
			a.x + t * (b.x - a.x),
			a.y + t * (b.y - a.y),
			a.z + t * (b.z - a.z),
			a.w + t * (b.w - a.w)
		};
	}

	// =============================================
	// Matrix4x4
	// =============================================

	[[nodiscard]] NU_FORCEINLINE NuMat4 SetIdentityMatrix4x4() noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = 1.0f; result.mat[0][1] = 0.0f; result.mat[0][2] = 0.0f; result.mat[0][3] = 0.0f;
		result.mat[1][0] = 0.0f; result.mat[1][1] = 1.0f; result.mat[1][2] = 0.0f; result.mat[1][3] = 0.0f;
		result.mat[2][0] = 0.0f; result.mat[2][1] = 0.0f; result.mat[2][2] = 1.0f; result.mat[2][3] = 0.0f;
		result.mat[3][0] = 0.0f; result.mat[3][1] = 0.0f; result.mat[3][2] = 0.0f; result.mat[3][3] = 1.0f;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Traspose
	[[nodiscard]] NU_FORCEINLINE NuMat4 Transpose(const NuMat4& m) noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = m.mat[0][0]; result.mat[1][0] = m.mat[0][1]; result.mat[2][0] = m.mat[0][2]; result.mat[3][0] = m.mat[0][3];
		result.mat[0][1] = m.mat[1][0]; result.mat[1][1] = m.mat[1][1]; result.mat[2][1] = m.mat[1][2]; result.mat[3][1] = m.mat[1][3];
		result.mat[0][2] = m.mat[2][0]; result.mat[1][2] = m.mat[2][1]; result.mat[2][2] = m.mat[2][2]; result.mat[3][2] = m.mat[2][3];
		result.mat[0][3] = m.mat[3][0]; result.mat[1][3] = m.mat[3][1]; result.mat[2][3] = m.mat[3][2]; result.mat[3][3] = m.mat[3][3];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuMat4 Add(const NuMat4& a, const NuMat4& b) noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = a.mat[0][0] + b.mat[0][0];
		result.mat[0][1] = a.mat[0][1] + b.mat[0][1];
		result.mat[0][2] = a.mat[0][2] + b.mat[0][2];
		result.mat[0][3] = a.mat[0][3] + b.mat[0][3];

		result.mat[1][0] = a.mat[1][0] + b.mat[1][0];
		result.mat[1][1] = a.mat[1][1] + b.mat[1][1];
		result.mat[1][2] = a.mat[1][2] + b.mat[1][2];
		result.mat[1][3] = a.mat[1][3] + b.mat[1][3];

		result.mat[2][0] = a.mat[2][0] + b.mat[2][0];
		result.mat[2][1] = a.mat[2][1] + b.mat[2][1];
		result.mat[2][2] = a.mat[2][2] + b.mat[2][2];
		result.mat[2][3] = a.mat[2][3] + b.mat[2][3];

		result.mat[3][0] = a.mat[3][0] + b.mat[3][0];
		result.mat[3][1] = a.mat[3][1] + b.mat[3][1];
		result.mat[3][2] = a.mat[3][2] + b.mat[3][2];
		result.mat[3][3] = a.mat[3][3] + b.mat[3][3];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuMat4 Sub(const NuMat4& a, const NuMat4& b) noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = a.mat[0][0] - b.mat[0][0];
		result.mat[0][1] = a.mat[0][1] - b.mat[0][1];
		result.mat[0][2] = a.mat[0][2] - b.mat[0][2];
		result.mat[0][3] = a.mat[0][3] - b.mat[0][3];

		result.mat[1][0] = a.mat[1][0] - b.mat[1][0];
		result.mat[1][1] = a.mat[1][1] - b.mat[1][1];
		result.mat[1][2] = a.mat[1][2] - b.mat[1][2];
		result.mat[1][3] = a.mat[1][3] - b.mat[1][3];

		result.mat[2][0] = a.mat[2][0] - b.mat[2][0];
		result.mat[2][1] = a.mat[2][1] - b.mat[2][1];
		result.mat[2][2] = a.mat[2][2] - b.mat[2][2];
		result.mat[2][3] = a.mat[2][3] - b.mat[2][3];

		result.mat[3][0] = a.mat[3][0] - b.mat[3][0];
		result.mat[3][1] = a.mat[3][1] - b.mat[3][1];
		result.mat[3][2] = a.mat[3][2] - b.mat[3][2];
		result.mat[3][3] = a.mat[3][3] - b.mat[3][3];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuMat4 Mul(const NuMat4& a, const NuMat4& b) noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = a.mat[0][0] * b.mat[0][0] + a.mat[0][1] * b.mat[1][0] + a.mat[0][2] * b.mat[2][0] + a.mat[0][3] * b.mat[3][0];
		result.mat[0][1] = a.mat[0][0] * b.mat[0][1] + a.mat[0][1] * b.mat[1][1] + a.mat[0][2] * b.mat[2][1] + a.mat[0][3] * b.mat[3][1];
		result.mat[0][2] = a.mat[0][0] * b.mat[0][2] + a.mat[0][1] * b.mat[1][2] + a.mat[0][2] * b.mat[2][2] + a.mat[0][3] * b.mat[3][2];
		result.mat[0][3] = a.mat[0][0] * b.mat[0][3] + a.mat[0][1] * b.mat[1][3] + a.mat[0][2] * b.mat[2][3] + a.mat[0][3] * b.mat[3][3];

		result.mat[1][0] = a.mat[1][0] * b.mat[0][0] + a.mat[1][1] * b.mat[1][0] + a.mat[1][2] * b.mat[2][0] + a.mat[1][3] * b.mat[3][0];
		result.mat[1][1] = a.mat[1][0] * b.mat[0][1] + a.mat[1][1] * b.mat[1][1] + a.mat[1][2] * b.mat[2][1] + a.mat[1][3] * b.mat[3][1];
		result.mat[1][2] = a.mat[1][0] * b.mat[0][2] + a.mat[1][1] * b.mat[1][2] + a.mat[1][2] * b.mat[2][2] + a.mat[1][3] * b.mat[3][2];
		result.mat[1][3] = a.mat[1][0] * b.mat[0][3] + a.mat[1][1] * b.mat[1][3] + a.mat[1][2] * b.mat[2][3] + a.mat[1][3] * b.mat[3][3];

		result.mat[2][0] = a.mat[2][0] * b.mat[0][0] + a.mat[2][1] * b.mat[1][0] + a.mat[2][2] * b.mat[2][0] + a.mat[2][3] * b.mat[3][0];
		result.mat[2][1] = a.mat[2][0] * b.mat[0][1] + a.mat[2][1] * b.mat[1][1] + a.mat[2][2] * b.mat[2][1] + a.mat[2][3] * b.mat[3][1];
		result.mat[2][2] = a.mat[2][0] * b.mat[0][2] + a.mat[2][1] * b.mat[1][2] + a.mat[2][2] * b.mat[2][2] + a.mat[2][3] * b.mat[3][2];
		result.mat[2][3] = a.mat[2][0] * b.mat[0][3] + a.mat[2][1] * b.mat[1][3] + a.mat[2][2] * b.mat[2][3] + a.mat[2][3] * b.mat[3][3];

		result.mat[3][0] = a.mat[3][0] * b.mat[0][0] + a.mat[3][1] * b.mat[1][0] + a.mat[3][2] * b.mat[2][0] + a.mat[3][3] * b.mat[3][0];
		result.mat[3][1] = a.mat[3][0] * b.mat[0][1] + a.mat[3][1] * b.mat[1][1] + a.mat[3][2] * b.mat[2][1] + a.mat[3][3] * b.mat[3][1];
		result.mat[3][2] = a.mat[3][0] * b.mat[0][2] + a.mat[3][1] * b.mat[1][2] + a.mat[3][2] * b.mat[2][2] + a.mat[3][3] * b.mat[3][2];
		result.mat[3][3] = a.mat[3][0] * b.mat[0][3] + a.mat[3][1] * b.mat[1][3] + a.mat[3][2] * b.mat[2][3] + a.mat[3][3] * b.mat[3][3];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuMat4& m, const NuVec4& v) noexcept
	{
		return {
			m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w,
			m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w,
			m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w,
			m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w,
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromRows
	[[nodiscard]] NU_FORCEINLINE NuMat4 FromRows(
		const NuVec4& r0, const NuVec4& r1, const NuVec4& r2, const NuVec4& r3) noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = r0.x; result.mat[0][1] = r0.y; result.mat[0][2] = r0.z; result.mat[0][3] = r0.w;
		result.mat[1][0] = r1.x; result.mat[1][1] = r1.y; result.mat[1][2] = r1.z; result.mat[1][3] = r1.w;
		result.mat[2][0] = r2.x; result.mat[2][1] = r2.y; result.mat[2][2] = r2.z; result.mat[2][3] = r2.w;
		result.mat[3][0] = r3.x; result.mat[3][1] = r3.y; result.mat[3][2] = r3.z; result.mat[3][3] = r3.w;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	[[nodiscard]] NU_FORCEINLINE NuMat4 FromColumns(
		const NuVec4& c0, const NuVec4& c1, const NuVec4& c2, const NuVec4& c3) noexcept
	{
		NuMat4 result{};

		result.mat[0][0] = c0.x; result.mat[1][0] = c0.y; result.mat[2][0] = c0.z; result.mat[3][0] = c0.w;
		result.mat[0][1] = c1.x; result.mat[1][1] = c1.y; result.mat[2][1] = c1.z; result.mat[3][1] = c1.w;
		result.mat[0][2] = c2.x; result.mat[1][2] = c2.y; result.mat[2][2] = c2.z; result.mat[3][2] = c2.w;
		result.mat[0][3] = c3.x; result.mat[1][3] = c3.y; result.mat[2][3] = c3.z; result.mat[3][3] = c3.w;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Determinant
	[[nodiscard]] NU_FORCEINLINE NuFloat Determinant(const NuMat4& m) noexcept
	{
		NuFloat det;

		NuFloat subFactor00 = m.mat[2][2] * m.mat[3][3] - m.mat[3][2] * m.mat[2][3];
		NuFloat subFactor01 = m.mat[2][1] * m.mat[3][3] - m.mat[3][1] * m.mat[2][3];
		NuFloat subFactor02 = m.mat[2][1] * m.mat[3][2] - m.mat[3][1] * m.mat[2][2];
		NuFloat subFactor03 = m.mat[2][0] * m.mat[3][3] - m.mat[3][0] * m.mat[2][3];
		NuFloat subFactor04 = m.mat[2][0] * m.mat[3][2] - m.mat[3][0] * m.mat[2][2];
		NuFloat subFactor05 = m.mat[2][0] * m.mat[3][1] - m.mat[3][0] * m.mat[2][1];

		NuFloat cof00 = +(m.mat[1][1] * subFactor00 - m.mat[1][2] * subFactor01 + m.mat[1][3] * subFactor02);
		NuFloat cof01 = -(m.mat[1][0] * subFactor00 - m.mat[1][2] * subFactor03 + m.mat[1][3] * subFactor04);
		NuFloat cof02 = +(m.mat[1][0] * subFactor01 - m.mat[1][1] * subFactor03 + m.mat[1][3] * subFactor05);
		NuFloat cof03 = -(m.mat[1][0] * subFactor02 - m.mat[1][1] * subFactor04 + m.mat[1][2] * subFactor05);

		det = m.mat[0][0] * cof00 + m.mat[0][1] * cof01 + m.mat[0][2] * cof02 + m.mat[0][3] * cof03;

		return det;
	}

	//copydoc NuEngine::Math::MatrixAPI::CreateTranslation
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateTranslation(const NuVec4& translation) noexcept
	{
		NuMat4 result = SetIdentityMatrix4x4();

		result.mat[0][3] = translation.x;
		result.mat[1][3] = translation.y;
		result.mat[2][3] = translation.z;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateScale
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateScale(const NuVec4& scale) noexcept
	{
		NuMat4 result = SetIdentityMatrix4x4();

		result.mat[3][0] = scale.x;
		result.mat[3][1] = scale.y;
		result.mat[3][2] = scale.z;

		return result;
	}

	//copydoc NuEngine::Math::MatrixAPI::GetColumn
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(const NuMat4& m, NuInt32 index) noexcept
	{
		NuEngine::Core::Types::NuAssert(index >= 0 && index < 4);
		return {
			m.mat[0][index], m.mat[1][index], m.mat[2][index], m.mat[3][index]
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(const NuMat4& m, NuInt32 index) noexcept
	{
		NuEngine::Core::Types::NuAssert(index >= 0 && index < 4);
		return { m.mat[index][0], m.mat[index][1], m.mat[index][2], m.mat[index][3] };
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	[[nodiscard]] NU_FORCEINLINE NuFloat Access(const NuMat4& m, NuInt32 row, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 4 && col >= 0 && col < 4);
		return m.mat[row][col];
	}

	// \copydoc NuEngine::Math::MatrixAPI::Data
	NU_FORCEINLINE const NuFloat* Data(const NuMat4& m) noexcept
	{
		return &m.mat[0][0];
	}

	// =============================================
	// Matrix3x3
	// =============================================

	// \copydoc NuEngine::Math::MatrixAPI::SetIdentityMatrix3x3
	[[nodiscard]] NU_FORCEINLINE NuMat3 SetIdentityMatrix3x3() noexcept
	{
		NuMat3 result{};

		result.mat[0][0] = 1.0f; result.mat[0][1] = 0.0f; result.mat[0][2] = 0.0f;
		result.mat[1][0] = 0.0f; result.mat[1][1] = 1.0f; result.mat[1][2] = 0.0f;
		result.mat[2][0] = 0.0f; result.mat[2][1] = 0.0f; result.mat[2][2] = 1.0f;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuMat3 Add(const NuMat3& a, const NuMat3& b) noexcept
	{
		NuMat3 result{};

		result.mat[0][0] = a.mat[0][0] + b.mat[0][0];
		result.mat[0][1] = a.mat[0][1] + b.mat[0][1];
		result.mat[0][2] = a.mat[0][2] + b.mat[0][2];

		result.mat[1][0] = a.mat[1][0] + b.mat[1][0];
		result.mat[1][1] = a.mat[1][1] + b.mat[1][1];
		result.mat[1][2] = a.mat[1][2] + b.mat[1][2];

		result.mat[2][0] = a.mat[2][0] + b.mat[2][0];
		result.mat[2][1] = a.mat[2][1] + b.mat[2][1];
		result.mat[2][2] = a.mat[2][2] + b.mat[2][2];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuMat3 Sub(const NuMat3& a, const NuMat3& b) noexcept
	{
		NuMat3 result{};

		result.mat[0][0] = a.mat[0][0] - b.mat[0][0];
		result.mat[0][1] = a.mat[0][1] - b.mat[0][1];
		result.mat[0][2] = a.mat[0][2] - b.mat[0][2];

		result.mat[1][0] = a.mat[1][0] - b.mat[1][0];
		result.mat[1][1] = a.mat[1][1] - b.mat[1][1];
		result.mat[1][2] = a.mat[1][2] - b.mat[1][2];

		result.mat[2][0] = a.mat[2][0] - b.mat[2][0];
		result.mat[2][1] = a.mat[2][1] - b.mat[2][1];
		result.mat[2][2] = a.mat[2][2] - b.mat[2][2];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuMat3 Mul(const NuMat3& a, const NuMat3& b) noexcept
	{
		NuMat3 result{};

		result.mat[0][0] = a.mat[0][0] * b.mat[0][0] + a.mat[0][1] * b.mat[1][0] + a.mat[0][2] * b.mat[2][0];
		result.mat[0][1] = a.mat[0][0] * b.mat[0][1] + a.mat[0][1] * b.mat[1][1] + a.mat[0][2] * b.mat[2][1];
		result.mat[0][2] = a.mat[0][0] * b.mat[0][2] + a.mat[0][1] * b.mat[1][2] + a.mat[0][2] * b.mat[2][2];

		result.mat[1][0] = a.mat[1][0] * b.mat[0][0] + a.mat[1][1] * b.mat[1][0] + a.mat[1][2] * b.mat[2][0];
		result.mat[1][1] = a.mat[1][0] * b.mat[0][1] + a.mat[1][1] * b.mat[1][1] + a.mat[1][2] * b.mat[2][1];
		result.mat[1][2] = a.mat[1][0] * b.mat[0][2] + a.mat[1][1] * b.mat[1][2] + a.mat[1][2] * b.mat[2][2];

		result.mat[2][0] = a.mat[2][0] * b.mat[0][0] + a.mat[2][1] * b.mat[1][0] + a.mat[2][2] * b.mat[2][0];
		result.mat[2][1] = a.mat[2][0] * b.mat[0][1] + a.mat[2][1] * b.mat[1][1] + a.mat[2][2] * b.mat[2][1];
		result.mat[2][2] = a.mat[2][0] * b.mat[0][2] + a.mat[2][1] * b.mat[1][2] + a.mat[2][2] * b.mat[2][2];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuMat3& m, const NuVec4& v) noexcept
	{
		return {
			m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z,
			m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z,
			m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z,
			0.0f,
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	[[nodiscard]] NU_FORCEINLINE NuMat3 FromColumns(const NuVec4& c0, const NuVec4& c1, const NuVec4& c2) noexcept
	{
		NuMat3 result{};

		result.mat[0][0] = c0.x; result.mat[1][0] = c0.y; result.mat[2][0] = c0.z;
		result.mat[0][1] = c1.x; result.mat[1][1] = c1.y; result.mat[2][1] = c1.z;
		result.mat[0][2] = c2.x; result.mat[1][2] = c2.y; result.mat[2][2] = c2.z;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromRows
	[[nodiscard]] NU_FORCEINLINE NuMat3 FromRows(const NuVec4& r0, const NuVec4& r1, const NuVec4& r2) noexcept
	{
		NuMat3 result{};

		result.mat[0][0] = r0.x; result.mat[0][1] = r0.y; result.mat[0][2] = r0.z;
		result.mat[1][0] = r1.x; result.mat[1][1] = r1.y; result.mat[1][2] = r1.z;
		result.mat[2][0] = r2.x; result.mat[2][1] = r2.y; result.mat[2][2] = r2.z;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetColumn
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(const NuMat3& m, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(col >= 0 && col < 3);
		return {
			m.mat[0][col], m.mat[1][col], m.mat[2][col], 0.0f
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(const NuMat3& m, NuInt32 row) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 3);
		return {
			m.mat[row][0], m.mat[row][1], m.mat[row][2], 0.0f
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	[[nodiscard]] NU_FORCEINLINE NuFloat Access(const NuMat3& m, NuInt32 row, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 3 && col >= 0 && col < 3);
		return m.mat[row][col];
	}

	// =============================================
	// Matrix2x2
	// =============================================

	// \copydoc NuEngine::Math::MatrixAPI::SetIdentityMatrix2x2
	[[nodiscard]] NU_FORCEINLINE NuMat2 SetIdentityMatrix2x2() noexcept
	{
		NuMat2 result{};

		result.mat[0][0] = 1.0f; result.mat[0][1] = 0.0f;
		result.mat[1][0] = 0.0f; result.mat[1][1] = 1.0f;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuMat2 Add(const NuMat2& a, const NuMat2& b) noexcept
	{
		NuMat2 result{};

		result.mat[0][0] = a.mat[0][0] + b.mat[0][0];
		result.mat[0][1] = a.mat[0][1] + b.mat[0][1];

		result.mat[1][0] = a.mat[1][0] + b.mat[1][0];
		result.mat[1][1] = a.mat[1][1] + b.mat[1][1];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuMat2 Sub(const NuMat2& a, const NuMat2& b) noexcept
	{
		NuMat2 result{};

		result.mat[0][0] = a.mat[0][0] - b.mat[0][0];
		result.mat[0][1] = a.mat[0][1] - b.mat[0][1];

		result.mat[1][0] = a.mat[1][0] - b.mat[1][0];
		result.mat[1][1] = a.mat[1][1] - b.mat[1][1];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuMat2 Mul(const NuMat2& a, const NuMat2& b) noexcept
	{
		NuMat2 result{};

		result.mat[0][0] = a.mat[0][0] * b.mat[0][0] + a.mat[0][1] * b.mat[1][0];
		result.mat[0][1] = a.mat[0][0] * b.mat[0][1] + a.mat[0][1] * b.mat[1][1];
		result.mat[1][0] = a.mat[1][0] * b.mat[0][0] + a.mat[1][1] * b.mat[1][0];
		result.mat[1][1] = a.mat[1][0] * b.mat[0][1] + a.mat[1][1] * b.mat[1][1];

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuMat2& m, const NuVec4& v) noexcept
	{
		return {
			m.mat[0][0] * v.x + m.mat[0][1] * v.y,
			m.mat[1][0] * v.x + m.mat[1][1] * v.y,
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	[[nodiscard]] NU_FORCEINLINE NuMat2 FromColumns(const NuVec4& c0, const NuVec4& c1) noexcept
	{
		NuMat2 result{};

		result.mat[0][0] = c0.x; result.mat[1][0] = c0.y;
		result.mat[0][1] = c1.x; result.mat[1][1] = c1.y;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	[[nodiscard]] NU_FORCEINLINE NuMat2 FromRows(const NuVec4& r0, const NuVec4& r1) noexcept
	{
		NuMat2 result{};

		result.mat[0][0] = r0.x; result.mat[0][1] = r0.y;
		result.mat[1][0] = r1.x; result.mat[1][1] = r1.y;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetColumn
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(const NuMat2& m, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(col >= 0 && col < 2);
		return {
			m.mat[0][col], m.mat[1][col], 0.0f, 0.0f
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(const NuMat2& m, NuInt32 row) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 2);
		return {
			m.mat[row][0], m.mat[row][1], 0.0f, 0.0f
		};
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	[[nodiscard]] NU_FORCEINLINE NuFloat Access(const NuMat2& m, NuInt32 row, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 2 && col >= 0 && col < 2);
		return m.mat[row][col];
	}
}