// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cmath>
#include <cassert>
#include <bit>

#include <Core/Types/Types.hpp>

namespace NuEngine::Math::Scalar
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
	* Represents a column-major 4x4 matrix stored as an array of 4 column vectors.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat4
	{
		NuVec4 cols[4];
	};

	/*
	* @brief 3x3 matrix type with 16-byte alignment.
	*
	* Represents a column-major 3x3 matrix stored as an array of 3 column vectors.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat3
	{
		NuVec4 cols[3];
	};

	/*
	* @brief 2x2 matrix type with 16-byte alignment.
	*
	* Represents a column-major 2x2 matrix stored as an array of 2 column vectors.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat2
	{
		NuVec4 cols[2];
	};

	// =============================================
	// Vectors
	// =============================================

	// \copydoc NuEngine::Math::VectorAPI::Set
	[[nodiscard]] NU_FORCEINLINE NuVec4 Set(NuFloat x, NuFloat y, NuFloat z = 0.0f, NuFloat w = 0.0f) noexcept
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
	[[nodiscard]] NU_FORCEINLINE NuFloat GetZ(const  NuVec4& v) noexcept
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

	// \copydoc NuEngine::Math::MatrixAPI::SetIdentityMatrix
	[[nodiscard]] NU_FORCEINLINE NuMat4 SetIdentityMatrix() noexcept
	{
		NuMat4 result{};

		result.cols[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		result.cols[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
		result.cols[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
		result.cols[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Traspose
	[[nodiscard]] NU_FORCEINLINE NuMat4 Transpose(const NuMat4& m) noexcept
	{
		NuMat4 result{};

		result.cols[0] = { m.cols[0].x, m.cols[1].x, m.cols[2].x, m.cols[3].x };
		result.cols[1] = { m.cols[0].y, m.cols[1].y, m.cols[2].y, m.cols[3].y };
		result.cols[2] = { m.cols[0].z, m.cols[1].z, m.cols[2].z, m.cols[3].z };
		result.cols[3] = { m.cols[0].w, m.cols[1].w, m.cols[2].w, m.cols[3].w };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuMat4 Add(const NuMat4& a, const NuMat4& b) noexcept
	{
		NuMat4 result{};

		result.cols[0] = Add(a.cols[0], b.cols[0]);
		result.cols[1] = Add(a.cols[1], b.cols[1]);
		result.cols[2] = Add(a.cols[2], b.cols[2]);
		result.cols[3] = Add(a.cols[3], b.cols[3]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuMat4 Sub(const NuMat4& a, const NuMat4& b) noexcept
	{
		NuMat4 result{};

		result.cols[0] = Sub(a.cols[0], b.cols[0]);
		result.cols[1] = Sub(a.cols[1], b.cols[1]);
		result.cols[2] = Sub(a.cols[2], b.cols[2]);
		result.cols[3] = Sub(a.cols[3], b.cols[3]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuMat4& m, const NuVec4& v) noexcept
	{
		NuVec4 r0 = Mul(m.cols[0], SetAll(v.x));
		NuVec4 r1 = Mul(m.cols[1], SetAll(v.y));
		NuVec4 r2 = Mul(m.cols[2], SetAll(v.z));
		NuVec4 r3 = Mul(m.cols[3], SetAll(v.w));

		return Add(Add(r0, r1), Add(r2, r3));
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuMat4 Mul(const NuMat4& a, const NuMat4& b) noexcept
	{
		NuMat4 result{};

		result.cols[0] = Mul(a, b.cols[0]);
		result.cols[1] = Mul(a, b.cols[1]);
		result.cols[2] = Mul(a, b.cols[2]);
		result.cols[3] = Mul(a, b.cols[3]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromRows
	[[nodiscard]] NU_FORCEINLINE NuMat4 FromRows(
		const NuVec4& r0, const NuVec4& r1, const NuVec4& r2, const NuVec4& r3) noexcept
	{
		NuMat4 result{};

		result.cols[0] = { r0.x, r1.x, r2.x, r3.x };
		result.cols[1] = { r0.y, r1.y, r2.y, r3.y };
		result.cols[2] = { r0.z, r1.z, r2.z, r3.z };
		result.cols[3] = { r0.w, r1.w, r2.w, r3.w };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	[[nodiscard]] NU_FORCEINLINE NuMat4 FromColumns(
		const NuVec4& c0, const NuVec4& c1, const NuVec4& c2, const NuVec4& c3) noexcept
	{
		NuMat4 result{};

		result.cols[0] = c0;
		result.cols[1] = c1;
		result.cols[2] = c2;
		result.cols[3] = c3;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Determinant
	[[nodiscard]] NU_FORCEINLINE NuFloat Determinant(const NuMat4& m) noexcept
	{
		NuFloat subFactor00 = m.cols[2].z * m.cols[3].w - m.cols[2].w * m.cols[3].z;
		NuFloat subFactor01 = m.cols[1].z * m.cols[3].w - m.cols[1].w * m.cols[3].z;
		NuFloat subFactor02 = m.cols[1].z * m.cols[2].w - m.cols[1].w * m.cols[2].z;
		NuFloat subFactor03 = m.cols[0].z * m.cols[3].w - m.cols[0].w * m.cols[3].z;
		NuFloat subFactor04 = m.cols[0].z * m.cols[2].w - m.cols[0].w * m.cols[2].z;
		NuFloat subFactor05 = m.cols[0].z * m.cols[1].w - m.cols[0].w * m.cols[1].z;

		NuFloat cof00 = +(m.cols[1].y * subFactor00 - m.cols[2].y * subFactor01 + m.cols[3].y * subFactor02);
		NuFloat cof01 = -(m.cols[0].y * subFactor00 - m.cols[2].y * subFactor03 + m.cols[3].y * subFactor04);
		NuFloat cof02 = +(m.cols[0].y * subFactor01 - m.cols[1].y * subFactor03 + m.cols[3].y * subFactor05);
		NuFloat cof03 = -(m.cols[0].y * subFactor02 - m.cols[1].y * subFactor04 + m.cols[2].y * subFactor05);

		NuFloat det = m.cols[0].x * cof00 + m.cols[1].x * cof01 + m.cols[2].x * cof02 + m.cols[3].x * cof03;

		return det;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Inverse
	[[nodiscard]] NU_FORCEINLINE NuMat4 Inverse(const NuMat4& m) noexcept
    {
        NuFloat sb_zw_00 = m.cols[2].z * m.cols[3].w - m.cols[2].w * m.cols[3].z;
        NuFloat sb_zw_01 = m.cols[1].z * m.cols[3].w - m.cols[1].w * m.cols[3].z;
        NuFloat sb_zw_02 = m.cols[1].z * m.cols[2].w - m.cols[1].w * m.cols[2].z;
        NuFloat sb_zw_03 = m.cols[0].z * m.cols[3].w - m.cols[0].w * m.cols[3].z;
        NuFloat sb_zw_04 = m.cols[0].z * m.cols[2].w - m.cols[0].w * m.cols[2].z;
        NuFloat sb_zw_05 = m.cols[0].z * m.cols[1].w - m.cols[0].w * m.cols[1].z;

        NuFloat sb_yw_00 = m.cols[2].y * m.cols[3].w - m.cols[2].w * m.cols[3].y;
        NuFloat sb_yw_01 = m.cols[1].y * m.cols[3].w - m.cols[1].w * m.cols[3].y;
        NuFloat sb_yw_02 = m.cols[1].y * m.cols[2].w - m.cols[1].w * m.cols[2].y;
        NuFloat sb_yw_03 = m.cols[0].y * m.cols[3].w - m.cols[0].w * m.cols[3].y;
        NuFloat sb_yw_04 = m.cols[0].y * m.cols[2].w - m.cols[0].w * m.cols[2].y;
        NuFloat sb_yw_05 = m.cols[0].y * m.cols[1].w - m.cols[0].w * m.cols[1].y;
        
        NuFloat sb_yz_00 = m.cols[2].y * m.cols[3].z - m.cols[2].z * m.cols[3].y;
        NuFloat sb_yz_01 = m.cols[1].y * m.cols[3].z - m.cols[1].z * m.cols[3].y;
        NuFloat sb_yz_02 = m.cols[1].y * m.cols[2].z - m.cols[1].z * m.cols[2].y;
        NuFloat sb_yz_03 = m.cols[0].y * m.cols[3].z - m.cols[0].z * m.cols[3].y;
        NuFloat sb_yz_04 = m.cols[0].y * m.cols[2].z - m.cols[0].z * m.cols[2].y;
        NuFloat sb_yz_05 = m.cols[0].y * m.cols[1].z - m.cols[0].z * m.cols[1].y;

        NuFloat cof00 = +(m.cols[1].y * sb_zw_00 - m.cols[2].y * sb_zw_01 + m.cols[3].y * sb_zw_02);
        NuFloat cof01 = -(m.cols[0].y * sb_zw_00 - m.cols[2].y * sb_zw_03 + m.cols[3].y * sb_zw_04);
        NuFloat cof02 = +(m.cols[0].y * sb_zw_01 - m.cols[1].y * sb_zw_03 + m.cols[3].y * sb_zw_05);
        NuFloat cof03 = -(m.cols[0].y * sb_zw_02 - m.cols[1].y * sb_zw_04 + m.cols[2].y * sb_zw_05);

        NuFloat cof10 = -(m.cols[1].x * sb_zw_00 - m.cols[2].x * sb_zw_01 + m.cols[3].x * sb_zw_02);
        NuFloat cof11 = +(m.cols[0].x * sb_zw_00 - m.cols[2].x * sb_zw_03 + m.cols[3].x * sb_zw_04);
        NuFloat cof12 = -(m.cols[0].x * sb_zw_01 - m.cols[1].x * sb_zw_03 + m.cols[3].x * sb_zw_05);
        NuFloat cof13 = +(m.cols[0].x * sb_zw_02 - m.cols[1].x * sb_zw_04 + m.cols[2].x * sb_zw_05);

        NuFloat cof20 = +(m.cols[1].x * sb_yw_00 - m.cols[2].x * sb_yw_01 + m.cols[3].x * sb_yw_02);
        NuFloat cof21 = -(m.cols[0].x * sb_yw_00 - m.cols[2].x * sb_yw_03 + m.cols[3].x * sb_yw_04);
        NuFloat cof22 = +(m.cols[0].x * sb_yw_01 - m.cols[1].x * sb_yw_03 + m.cols[3].x * sb_yw_05);
        NuFloat cof23 = -(m.cols[0].x * sb_yw_02 - m.cols[1].x * sb_yw_04 + m.cols[2].x * sb_yw_05);

        NuFloat cof30 = -(m.cols[1].x * sb_yz_00 - m.cols[2].x * sb_yz_01 + m.cols[3].x * sb_yz_02);
        NuFloat cof31 = +(m.cols[0].x * sb_yz_00 - m.cols[2].x * sb_yz_03 + m.cols[3].x * sb_yz_04);
        NuFloat cof32 = -(m.cols[0].x * sb_yz_01 - m.cols[1].x * sb_yz_03 + m.cols[3].x * sb_yz_05);
        NuFloat cof33 = +(m.cols[0].x * sb_yz_02 - m.cols[1].x * sb_yz_04 + m.cols[2].x * sb_yz_05);

        NuFloat det = m.cols[0].x * cof00 + m.cols[1].x * cof01 + m.cols[2].x * cof02 + m.cols[3].x * cof03;

        Core::Types::NuAssert(std::fabs(det) > 1e-8f, "Matrix is non-invertible (determinant is zero)!");

        NuFloat invDet = 1.0f / det;
        NuMat4 result{};
        
        result.cols[0] = { cof00 * invDet, cof01 * invDet, cof02 * invDet, cof03 * invDet };
        result.cols[1] = { cof10 * invDet, cof11 * invDet, cof12 * invDet, cof13 * invDet };
        result.cols[2] = { cof20 * invDet, cof21 * invDet, cof22 * invDet, cof23 * invDet };
        result.cols[3] = { cof30 * invDet, cof31 * invDet, cof32 * invDet, cof33 * invDet };

        return result;
    }

	// \copydoc NuEngine::Math::MatrixAPI::CreateTranslation
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateTranslation(const NuVec4& translation) noexcept
	{
		NuMat4 result = SetIdentityMatrix();

		result.cols[3] = { translation.x, translation.y, translation.z, 1.0f };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateScale
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateScale(const NuVec4& scale) noexcept
	{
		NuMat4 result = SetIdentityMatrix();

		result.cols[0].x = scale.x;
		result.cols[1].y = scale.y;
		result.cols[2].z = scale.z;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateRotation
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateRotation(const NuVec4& quat) noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateLookAt
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateLookAt(const NuVec4& eye, const NuVec4& target, const NuVec4& up) noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPI::CreatePerspective
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreatePerspective(NuFloat fovY, NuFloat aspect, NuFloat nearZ, NuFloat farZ) noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateOrthographic
	[[nodiscard]] NU_FORCEINLINE NuMat4 CreateOrthographic() noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPi::Decompose
	NU_FORCEINLINE void Decompose() noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPI::Equal
	[[nodiscard]] NU_FORCEINLINE Equal(const NuMat4& a, const NuMat4& b) noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPI::IsIdentity
	[[nodiscard]] NU_FORCEINLINE IsIdentity(const NuMat4& m, NuFloat epsilon) noexcept
	{

	}

	// \copydoc NuEngine::Math::MatrixAPI::GetColumn
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(const NuMat4& m, NuInt32 index) noexcept
	{
		NuEngine::Core::Types::NuAssert(index >= 0 && index < 4);
		return m.cols[index];
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(const NuMat4& m, NuInt32 index) noexcept
	{
		NuEngine::Core::Types::NuAssert(index >= 0 && index < 4);

		if (index == 0) return { m.cols[0].x, m.cols[1].x, m.cols[2].x, m.cols[3].x };
		if (index == 1) return { m.cols[0].y, m.cols[1].y, m.cols[2].y, m.cols[3].y };
		if (index == 2) return { m.cols[0].z, m.cols[1].z, m.cols[2].z, m.cols[3].z };

		return { m.cols[0].w, m.cols[1].w, m.cols[2].w, m.cols[3].w };
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	[[nodiscard]] NU_FORCEINLINE NuFloat Access(const NuMat4& m, NuInt32 row, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 4 && col >= 0 && col < 4);

		const NuVec4& column = m.cols[col];
		if (row == 0) return column.x;
		if (row == 1) return column.y;
		if (row == 2) return column.z;

		return column.w;
	}

	NU_FORCEINLINE const NuFloat* Data(const NuMat4& m) noexcept
	{
		return &m.cols[0].x;
	}

	// =============================================
	// Matrix3x3
	// =============================================

	// \copydoc NuEngine::Math::MatrixAPI::SetIdentityMatrix3x3
	[[nodiscard]] NU_FORCEINLINE NuMat3 SetIdentityMatrix3x3() noexcept
	{
		NuMat3 result{};

		result.cols[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		result.cols[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
		result.cols[2] = { 0.0f, 0.0f, 1.0f, 0.0f };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuMat3 Add(const NuMat3& a, const NuMat3& b) noexcept
	{
		NuMat3 result{};

		result.cols[0] = Add(a.cols[0], b.cols[0]);
		result.cols[1] = Add(a.cols[1], b.cols[1]);
		result.cols[2] = Add(a.cols[2], b.cols[2]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuMat3 Sub(const NuMat3& a, const NuMat3& b) noexcept
	{
		NuMat3 result{};

		result.cols[0] = Sub(a.cols[0], b.cols[0]);
		result.cols[1] = Sub(a.cols[1], b.cols[1]);
		result.cols[2] = Sub(a.cols[2], b.cols[2]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuMat3& m, const NuVec4& v) noexcept
	{
		NuVec4 r0 = Mul(m.cols[0], SetAll(v.x));
		NuVec4 r1 = Mul(m.cols[1], SetAll(v.y));
		NuVec4 r2 = Mul(m.cols[2], SetAll(v.z));

		return Add(Add(r0, r1), r2);
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuMat3 Mul(const NuMat3& a, const NuMat3& b) noexcept
	{
		NuMat3 result{};

		result.cols[0] = Mul(a, b.cols[0]);
		result.cols[1] = Mul(a, b.cols[1]);
		result.cols[2] = Mul(a, b.cols[2]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	[[nodiscard]] NU_FORCEINLINE NuMat3 FromColumns(const NuVec4& c0, const NuVec4& c1, const NuVec4& c2) noexcept
	{
		NuMat3 result{};

		result.cols[0] = c0;
		result.cols[1] = c1;
		result.cols[2] = c2;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromRows
	[[nodiscard]] NU_FORCEINLINE NuMat3 FromRows(const NuVec4& r0, const NuVec4& r1, const NuVec4& r2) noexcept
	{
		NuMat3 result{};

		result.cols[0] = { r0.x, r1.x, r2.x, 0.0f };
		result.cols[1] = { r0.y, r1.y, r2.y, 0.0f };
		result.cols[2] = { r0.z, r1.z, r2.z, 0.0f };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetColumn
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(const NuMat3& m, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(col >= 0 && col < 3);
		return m.cols[col];
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(const NuMat3& m, NuInt32 row) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 3);
		if (row == 0) return { m.cols[0].x, m.cols[1].x, m.cols[2].x, 0.0f };
		if (row == 1) return { m.cols[0].y, m.cols[1].y, m.cols[2].y, 0.0f };
		return { m.cols[0].z, m.cols[1].z, m.cols[2].z, 0.0f };
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	[[nodiscard]] NU_FORCEINLINE NuFloat Access(const NuMat3& m, NuInt32 row, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 3 && col >= 0 && col < 3);

		const NuVec4& column = m.cols[col];
		if (row == 0) return column.x;
		if (row == 1) return column.y;
		return column.z;
	}

	// =============================================
	// Matrix2x2
	// =============================================

	// \copydoc NuEngine::Math::MatrixAPI::SetIdentityMatrix2x2
	[[nodiscard]] NU_FORCEINLINE NuMat2 SetIdentityMatrix2x2() noexcept
	{
		NuMat2 result{};

		result.cols[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		result.cols[1] = { 0.0f, 1.0f, 0.0f, 0.0f };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuMat2 Add(const NuMat2& a, const NuMat2& b) noexcept
	{
		NuMat2 result{};

		result.cols[0] = Add(a.cols[0], b.cols[0]);
		result.cols[1] = Add(a.cols[1], b.cols[1]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuMat2 Sub(const NuMat2& a, const NuMat2& b) noexcept
	{
		NuMat2 result{};

		result.cols[0] = Sub(a.cols[0], b.cols[0]);
		result.cols[1] = Sub(a.cols[1], b.cols[1]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuMat2& m, const NuVec4& v) noexcept
	{
		NuVec4 r0 = Mul(m.cols[0], SetAll(v.x));
		NuVec4 r1 = Mul(m.cols[1], SetAll(v.y));
		return Add(r0, r1);
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuMat2 Mul(const NuMat2& a, const NuMat2& b) noexcept
	{
		NuMat2 result{};

		result.cols[0] = Mul(a, b.cols[0]);
		result.cols[1] = Mul(a, b.cols[1]);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	[[nodiscard]] NU_FORCEINLINE NuMat2 FromColumns(const NuVec4& c0, const NuVec4& c1) noexcept
	{
		NuMat2 result{};

		result.cols[0] = c0;
		result.cols[1] = c1;

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	[[nodiscard]] NU_FORCEINLINE NuMat2 FromRows(const NuVec4& r0, const NuVec4& r1) noexcept
	{
		NuMat2 result{};

		result.cols[0] = { r0.x, r1.x, 0.0f, 0.0f };
		result.cols[1] = { r0.y, r1.y, 0.0f, 0.0f };

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetColumn
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetColumn(const NuMat2& m, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(col >= 0 && col < 2);
		return m.cols[col];
	}

	// \copydoc NuEngine::Math::MatrixAPI
	[[nodiscard]] NU_FORCEINLINE NuVec4 GetRow(const NuMat2& m, NuInt32 row) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 2);
		if (row == 0) return { m.cols[0].x, m.cols[1].x, 0.0f, 0.0f };
		return { m.cols[0].y, m.cols[1].y, 0.0f, 0.0f };
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	[[nodiscard]] NU_FORCEINLINE NuFloat Access(const NuMat2& m, NuInt32 row, NuInt32 col) noexcept
	{
		NuEngine::Core::Types::NuAssert(row >= 0 && row < 2 && col >= 0 && col < 2);

		const NuVec4& column = m.cols[col];
		if (row == 0) return column.x;
		return column.y;
	}
}