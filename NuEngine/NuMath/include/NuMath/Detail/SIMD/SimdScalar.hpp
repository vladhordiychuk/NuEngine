// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/MathConstants.hpp>
#include <NuMath/Core/Math.hpp>

#include <bit>

namespace NuMath::Detail
{
	struct Scalar_Traits
	{
		// =============================================
		// Common types
		// =============================================

		/**
		 * @brief 4-component vector type with 16-byte alignment.
		 *
		 * Represents a SIMD-friendly vector with four float components (x, y, z, w).
		 * The structure is aligned to 16 bytes for optimal SIMD operations.
		 */
		struct alignas(16) NuVec4
		{
			float x, y, z, w;
		};

		using Register = NuVec4;

		static constexpr int Width = 4;

		/**
		 * @brief 4x4 matrix type with 16-byte alignment.
		 *
		 * Represents a column-major 4x4 matrix stored as an array of 4 column vectors.
		 * The structure is aligned to 16 bytes for optimal SIMD operations.
		 */
		struct alignas(16) NuMat4
		{
			NuVec4 cols[4];
		};

		/**
		 * @brief 3x3 matrix type with 16-byte alignment.
		 *
		 * Represents a column-major 3x3 matrix stored as an array of 3 column vectors.
		 * The structure is aligned to 16 bytes for optimal SIMD operations.
		 */
		struct alignas(16) NuMat3
		{
			NuVec4 cols[3];
		};

		/**
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

		// \coptydoc NuMath::VectorAPI::Load
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Load(const NuVecStorage4& vec) noexcept
		{
			return { vec.x, vec.y, vec.z, vec.w };
		}

		// \copydoc NuMath::VectorAPI::Store
		static NU_FORCEINLINE void Store(NuVecStorage4& vec, NuVec4 val) noexcept
		{
			vec.x = val.x; vec.y = val.y; vec.z = val.z; vec.w = val.w;
		}

		// \copydoc NuMath::VectorAPI::Set
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Set(float x, float y, float z = 0.0f, float w = 0.0f) noexcept
		{
			return { x, y, z, w };
		}

		// \copydoc NuMath::VectorAPI::SetAll
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetAll(float scalar) noexcept
		{
			return { scalar, scalar, scalar, scalar };
		}

		// \copydoc NuMath::VectorAPI::SetZero
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetZero() noexcept
		{
			return { 0.0f, 0.0f, 0.0f, 0.0f };
		}

		// \copydoc NuMath::VectorAPI::GetX
		[[nodiscard]] static NU_FORCEINLINE float GetX(const NuVec4& v) noexcept
		{
			return v.x;
		}

		// \copydoc NuMath::VectorAPI::GetY
		[[nodiscard]] static NU_FORCEINLINE float GetY(const NuVec4& v) noexcept
		{
			return v.y;
		}

		// \copydoc NuMath::VectorAPI::GetZ
		[[nodiscard]] static NU_FORCEINLINE float GetZ(const NuVec4& v) noexcept
		{
			return v.z;
		}

		// \copydoc NuMath::VectorAPI::GetW
		[[nodiscard]] static NU_FORCEINLINE float GetW(const NuVec4& v) noexcept
		{
			return v.w;
		}

		// \copydoc NuMath::VectorAPI::SetX
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetX(NuVec4 v, float x) noexcept
		{
			v.x = x;
			return v;
		}

		// \copydoc NuMath::VectorAPI::SetY
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetY(NuVec4 v, float y) noexcept
		{
			v.y = y;
			return v;
		}

		// \copydoc NuMath::VectorAPI::SetZ
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetZ(NuVec4 v, float z) noexcept
		{
			v.z = z;
			return v;
		}

		// \copydoc NuMath::VectorAPI::SetW
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetW(NuVec4 v, float w) noexcept
		{
			v.w = w;
			return v;
		}

		// \copydoc NuMath::VectorAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Add(const NuVec4& a, const NuVec4& b) noexcept
		{
			return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
		}

		// \copydoc NuMath::VectorAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Sub(const NuVec4& a, const NuVec4& b) noexcept
		{
			return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
		}

		// \copydoc NuMath::VectorAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuVec4& a, const NuVec4& b) noexcept
		{
			return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
		}

		// \copydoc NuMath::VectorAPI::Div
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Div(const NuVec4& a, const NuVec4& b) noexcept
		{
			NU_MATH_ASSERT(std::fabs(b.x) > EPSILON, "Vector division by zero (x)");
			NU_MATH_ASSERT(std::fabs(b.y) > EPSILON, "Vector division by zero (y)");
			NU_MATH_ASSERT(std::fabs(b.z) > EPSILON, "Vector division by zero (z)");
			NU_MATH_ASSERT(std::fabs(b.w) > EPSILON, "Vector division by zero (w)");

			return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
		}

		// \copydoc NuMath::VectorAPI::Neg
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Neg(const NuVec4& a) noexcept
		{
			return { -a.x, -a.y, -a.z, -a.w };
		}

		// \copydoc NuMath::VectorAPI::Min
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Min(const NuVec4& a, const NuVec4& b) noexcept
		{
			return {
				(a.x < b.x) ? a.x : b.x,
				(a.y < b.y) ? a.y : b.y,
				(a.z < b.z) ? a.z : b.z,
				(a.w < b.w) ? a.w : b.w
			};
		}

		// \copydoc NuMath::VectorAPI::Max
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Max(const NuVec4& a, const NuVec4& b) noexcept
		{
			return {
				(a.x > b.x) ? a.x : b.x,
				(a.y > b.y) ? a.y : b.y,
				(a.z > b.z) ? a.z : b.z,
				(a.w > b.w) ? a.w : b.w
			};
		}

		// \copydoc NuMath::VectorAPI::Abs
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Abs(const NuVec4& v) noexcept
		{
			return { std::fabs(v.x), std::fabs(v.y), std::fabs(v.z), std::fabs(v.w) };
		}

		// \copydoc NuMath::VectorAPI::Equal
		[[nodiscard]] static NU_FORCEINLINE bool Equal(const NuVec4& a, const NuVec4& b) noexcept
		{
			return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
		}

		// \copydoc NuMath::VectorAPI::NearEqual
		[[nodiscard]] static NU_FORCEINLINE bool NearEqual(const NuVec4& a, const NuVec4& b, float epsilon) noexcept
		{
			return (std::fabs(a.x - b.x) <= epsilon) &&
				(std::fabs(a.y - b.y) <= epsilon) &&
				(std::fabs(a.z - b.z) <= epsilon) &&
				(std::fabs(a.w - b.w) <= epsilon);
		}

		// \copydoc NuMath::VectorAPI::SqrtScalar
		[[nodiscard]] static NU_FORCEINLINE float SqrtScalar(float scalar) noexcept
		{
			return std::sqrt(scalar);
		}

		// \copydoc NuMath::VectorAPI::HorizontalAdd4
		[[nodiscard]] static NU_FORCEINLINE float HorizontalAdd4(const NuVec4& v) noexcept
		{
			return v.x + v.y + v.z + v.w;
		}

		// \copydoc NuMath::VectorAPI::HorizontalAdd3
		[[nodiscard]] static NU_FORCEINLINE float HorizontalAdd3(const NuVec4& v) noexcept
		{
			return v.x + v.y + v.z;
		}

		// \copydoc NuMath::VectorAPI::HorizontalAdd2
		[[nodiscard]] static NU_FORCEINLINE float HorizontalAdd2(const NuVec4& v) noexcept
		{
			return v.x + v.y;
		}

		[[nodiscard]] static NU_FORCEINLINE float InvSqrtFast(float x) noexcept
		{
			const float xhalf = 0.5f * x;
			int i = std::bit_cast<int>(x);
			i = 0x5f3759df - (i >> 1);
			float result = std::bit_cast<float>(i);
			result = result * (1.5f - xhalf * result * result);
			return result;
		}

		// \copydoc NuMath::VectorAPI::Normalize2
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize2(const NuVec4& v) noexcept
		{
			float lenSq = v.x * v.x + v.y * v.y;
			NU_MATH_ASSERT(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
			float invLen = InvSqrtFast(lenSq);
			return { v.x * invLen, v.y * invLen, v.z, v.w };
		}

		// \copydoc NuMath::VectorAPI::Normalize3
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize3(const NuVec4& v) noexcept
		{
			const float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
			NU_MATH_ASSERT(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
			const float invLen = InvSqrtFast(lenSq);
			return { v.x * invLen, v.y * invLen, v.z * invLen, v.w };
		}

		// \copydoc NuMath::VectorAPI::Normalize4
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize4(const NuVec4& v) noexcept
		{
			float lenSq = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
			NU_MATH_ASSERT(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
			float invLength = InvSqrtFast(lenSq);
			return { v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength };
		}

		// \copydoc NuMath::VectorAPI::Cross
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Cross(const NuVec4& a, const NuVec4& b) noexcept
		{
			return {
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x,
				0.0f
			};
		}

		// \copydoc NuMath::VectorAPI::Dot2
		[[nodiscard]] static NU_FORCEINLINE float Dot2(const NuVec4& a, const NuVec4& b) noexcept
		{
			return a.x * b.x + a.y * b.y;
		}

		// \copydoc NuMath::VectorAPI::Dot3
		[[nodiscard]] static NU_FORCEINLINE float Dot3(const NuVec4& a, const NuVec4& b) noexcept
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		// \copydoc NuMath::VectorAPI::Dot4
		[[nodiscard]] static NU_FORCEINLINE float Dot4(const NuVec4& a, const NuVec4& b) noexcept
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		// \copydoc NuMath::VectorAPI::Length2
		[[nodiscard]] static NU_FORCEINLINE float Length2(const NuVec4& v) noexcept
		{
			return SqrtScalar(Dot2(v, v));
		}

		// \copydoc NuMath::VectorAPI::Length3
		[[nodiscard]] static NU_FORCEINLINE float Length3(const NuVec4& v) noexcept
		{
			return SqrtScalar(Dot3(v, v));
		}

		// \copydoc NuMath::VectorAPI::Length4
		[[nodiscard]] static NU_FORCEINLINE float Length4(const NuVec4& v) noexcept
		{
			return SqrtScalar(Dot4(v, v));
		}

		// \copydoc NuMath::VectorAPI::Lerp
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Lerp(const NuVec4& a, const NuVec4& b, float t) noexcept
		{
			return {
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z),
				a.w + t * (b.w - a.w)
			};
		}

		template <int I0, int I1, int I2, int I3>
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Shuffle(const NuVec4& v) noexcept
		{
			const float arr[] = { v.x, v.y, v.z, v.w };
			return { arr[I0], arr[I1], arr[I2], arr[I3] };
		}

		// =============================================
		// Matrix4x4
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix
		[[nodiscard]] static NU_FORCEINLINE NuMat4 SetIdentityMatrix() noexcept
		{
			NuMat4 result{};

			result.cols[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
			result.cols[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
			result.cols[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
			result.cols[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Traspose
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Transpose(const NuMat4& m) noexcept
		{
			NuMat4 result{};

			result.cols[0] = { m.cols[0].x, m.cols[1].x, m.cols[2].x, m.cols[3].x };
			result.cols[1] = { m.cols[0].y, m.cols[1].y, m.cols[2].y, m.cols[3].y };
			result.cols[2] = { m.cols[0].z, m.cols[1].z, m.cols[2].z, m.cols[3].z };
			result.cols[3] = { m.cols[0].w, m.cols[1].w, m.cols[2].w, m.cols[3].w };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Add(const NuMat4& a, const NuMat4& b) noexcept
		{
			NuMat4 result{};

			result.cols[0] = Add(a.cols[0], b.cols[0]);
			result.cols[1] = Add(a.cols[1], b.cols[1]);
			result.cols[2] = Add(a.cols[2], b.cols[2]);
			result.cols[3] = Add(a.cols[3], b.cols[3]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Sub(const NuMat4& a, const NuMat4& b) noexcept
		{
			NuMat4 result{};

			result.cols[0] = Sub(a.cols[0], b.cols[0]);
			result.cols[1] = Sub(a.cols[1], b.cols[1]);
			result.cols[2] = Sub(a.cols[2], b.cols[2]);
			result.cols[3] = Sub(a.cols[3], b.cols[3]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuMat4& m, const NuVec4& v) noexcept
		{
			NuVec4 r0 = Mul(m.cols[0], SetAll(v.x));
			NuVec4 r1 = Mul(m.cols[1], SetAll(v.y));
			NuVec4 r2 = Mul(m.cols[2], SetAll(v.z));
			NuVec4 r3 = Mul(m.cols[3], SetAll(v.w));

			return Add(Add(r0, r1), Add(r2, r3));
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Mul(const NuMat4& a, const NuMat4& b) noexcept
		{
			NuMat4 result{};

			result.cols[0] = Mul(a, b.cols[0]);
			result.cols[1] = Mul(a, b.cols[1]);
			result.cols[2] = Mul(a, b.cols[2]);
			result.cols[3] = Mul(a, b.cols[3]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat4 FromRows(
			const NuVec4& r0, const NuVec4& r1, const NuVec4& r2, const NuVec4& r3) noexcept
		{
			NuMat4 result{};

			result.cols[0] = { r0.x, r1.x, r2.x, r3.x };
			result.cols[1] = { r0.y, r1.y, r2.y, r3.y };
			result.cols[2] = { r0.z, r1.z, r2.z, r3.z };
			result.cols[3] = { r0.w, r1.w, r2.w, r3.w };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat4 FromColumns(
			const NuVec4& c0, const NuVec4& c1, const NuVec4& c2, const NuVec4& c3) noexcept
		{
			NuMat4 result{};

			result.cols[0] = c0;
			result.cols[1] = c1;
			result.cols[2] = c2;
			result.cols[3] = c3;

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Determinant
		[[nodiscard]] static NU_FORCEINLINE float Determinant(const NuMat4& m) noexcept
		{
			float subFactor00 = m.cols[2].z * m.cols[3].w - m.cols[2].w * m.cols[3].z;
			float subFactor01 = m.cols[1].z * m.cols[3].w - m.cols[1].w * m.cols[3].z;
			float subFactor02 = m.cols[1].z * m.cols[2].w - m.cols[1].w * m.cols[2].z;
			float subFactor03 = m.cols[0].z * m.cols[3].w - m.cols[0].w * m.cols[3].z;
			float subFactor04 = m.cols[0].z * m.cols[2].w - m.cols[0].w * m.cols[2].z;
			float subFactor05 = m.cols[0].z * m.cols[1].w - m.cols[0].w * m.cols[1].z;

			float cof00 = +(m.cols[1].y * subFactor00 - m.cols[2].y * subFactor01 + m.cols[3].y * subFactor02);
			float cof01 = -(m.cols[0].y * subFactor00 - m.cols[2].y * subFactor03 + m.cols[3].y * subFactor04);
			float cof02 = +(m.cols[0].y * subFactor01 - m.cols[1].y * subFactor03 + m.cols[3].y * subFactor05);
			float cof03 = -(m.cols[0].y * subFactor02 - m.cols[1].y * subFactor04 + m.cols[2].y * subFactor05);

			float det = m.cols[0].x * cof00 + m.cols[1].x * cof01 + m.cols[2].x * cof02 + m.cols[3].x * cof03;

			return det;
		}

		// \copydoc NuMath::MatrixAPI::Inverse
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Inverse(const NuMat4& m) noexcept
		{
			float sb_zw_00 = m.cols[2].z * m.cols[3].w - m.cols[2].w * m.cols[3].z;
			float sb_zw_01 = m.cols[1].z * m.cols[3].w - m.cols[1].w * m.cols[3].z;
			float sb_zw_02 = m.cols[1].z * m.cols[2].w - m.cols[1].w * m.cols[2].z;
			float sb_zw_03 = m.cols[0].z * m.cols[3].w - m.cols[0].w * m.cols[3].z;
			float sb_zw_04 = m.cols[0].z * m.cols[2].w - m.cols[0].w * m.cols[2].z;
			float sb_zw_05 = m.cols[0].z * m.cols[1].w - m.cols[0].w * m.cols[1].z;

			float sb_yw_00 = m.cols[2].y * m.cols[3].w - m.cols[2].w * m.cols[3].y;
			float sb_yw_01 = m.cols[1].y * m.cols[3].w - m.cols[1].w * m.cols[3].y;
			float sb_yw_02 = m.cols[1].y * m.cols[2].w - m.cols[1].w * m.cols[2].y;
			float sb_yw_03 = m.cols[0].y * m.cols[3].w - m.cols[0].w * m.cols[3].y;
			float sb_yw_04 = m.cols[0].y * m.cols[2].w - m.cols[0].w * m.cols[2].y;
			float sb_yw_05 = m.cols[0].y * m.cols[1].w - m.cols[0].w * m.cols[1].y;

			float sb_yz_00 = m.cols[2].y * m.cols[3].z - m.cols[2].z * m.cols[3].y;
			float sb_yz_01 = m.cols[1].y * m.cols[3].z - m.cols[1].z * m.cols[3].y;
			float sb_yz_02 = m.cols[1].y * m.cols[2].z - m.cols[1].z * m.cols[2].y;
			float sb_yz_03 = m.cols[0].y * m.cols[3].z - m.cols[0].z * m.cols[3].y;
			float sb_yz_04 = m.cols[0].y * m.cols[2].z - m.cols[0].z * m.cols[2].y;
			float sb_yz_05 = m.cols[0].y * m.cols[1].z - m.cols[0].z * m.cols[1].y;

			float cof00 = +(m.cols[1].y * sb_zw_00 - m.cols[2].y * sb_zw_01 + m.cols[3].y * sb_zw_02);
			float cof01 = -(m.cols[0].y * sb_zw_00 - m.cols[2].y * sb_zw_03 + m.cols[3].y * sb_zw_04);
			float cof02 = +(m.cols[0].y * sb_zw_01 - m.cols[1].y * sb_zw_03 + m.cols[3].y * sb_zw_05);
			float cof03 = -(m.cols[0].y * sb_zw_02 - m.cols[1].y * sb_zw_04 + m.cols[2].y * sb_zw_05);

			float cof10 = -(m.cols[1].x * sb_zw_00 - m.cols[2].x * sb_zw_01 + m.cols[3].x * sb_zw_02);
			float cof11 = +(m.cols[0].x * sb_zw_00 - m.cols[2].x * sb_zw_03 + m.cols[3].x * sb_zw_04);
			float cof12 = -(m.cols[0].x * sb_zw_01 - m.cols[1].x * sb_zw_03 + m.cols[3].x * sb_zw_05);
			float cof13 = +(m.cols[0].x * sb_zw_02 - m.cols[1].x * sb_zw_04 + m.cols[2].x * sb_zw_05);

			float cof20 = +(m.cols[1].x * sb_yw_00 - m.cols[2].x * sb_yw_01 + m.cols[3].x * sb_yw_02);
			float cof21 = -(m.cols[0].x * sb_yw_00 - m.cols[2].x * sb_yw_03 + m.cols[3].x * sb_yw_04);
			float cof22 = +(m.cols[0].x * sb_yw_01 - m.cols[1].x * sb_yw_03 + m.cols[3].x * sb_yw_05);
			float cof23 = -(m.cols[0].x * sb_yw_02 - m.cols[1].x * sb_yw_04 + m.cols[2].x * sb_yw_05);

			float cof30 = -(m.cols[1].x * sb_yz_00 - m.cols[2].x * sb_yz_01 + m.cols[3].x * sb_yz_02);
			float cof31 = +(m.cols[0].x * sb_yz_00 - m.cols[2].x * sb_yz_03 + m.cols[3].x * sb_yz_04);
			float cof32 = -(m.cols[0].x * sb_yz_01 - m.cols[1].x * sb_yz_03 + m.cols[3].x * sb_yz_05);
			float cof33 = +(m.cols[0].x * sb_yz_02 - m.cols[1].x * sb_yz_04 + m.cols[2].x * sb_yz_05);

			float det = m.cols[0].x * cof00 + m.cols[1].x * cof01 + m.cols[2].x * cof02 + m.cols[3].x * cof03;

			NU_MATH_ASSERT(std::fabs(det) > 1e-8f, "Matrix is non-invertible (determinant is zero)!");

			float invDet = 1.0f / det;
			NuMat4 result{};

			result.cols[0] = { cof00 * invDet, cof01 * invDet, cof02 * invDet, cof03 * invDet };
			result.cols[1] = { cof10 * invDet, cof11 * invDet, cof12 * invDet, cof13 * invDet };
			result.cols[2] = { cof20 * invDet, cof21 * invDet, cof22 * invDet, cof23 * invDet };
			result.cols[3] = { cof30 * invDet, cof31 * invDet, cof32 * invDet, cof33 * invDet };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateTranslation
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateTranslation(const NuVec4& translation) noexcept
		{
			NuMat4 result = SetIdentityMatrix();

			result.cols[3] = { translation.x, translation.y, translation.z, 1.0f };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateScale
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateScale(const NuVec4& scale) noexcept
		{
			NuMat4 result = SetIdentityMatrix();

			result.cols[0].x = scale.x;
			result.cols[1].y = scale.y;
			result.cols[2].z = scale.z;

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateRotation
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateRotation(const NuVec4& quat) noexcept
		{
			return SetIdentityMatrix();
			// TO DO
		}

		// \copydoc NuMath::MatrixAPI::CreateLookAt
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateLookAt(const NuVec4& eye, const NuVec4& target, const NuVec4& up) noexcept
		{
			return SetIdentityMatrix();
		}

		// \copydoc NuMath::MatrixAPI::CreatePerspective
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreatePerspective(float fovY, float aspect, float nearZ, float farZ) noexcept
		{
			return SetIdentityMatrix();
			// TO DO
		}

		// \copydoc NuMath::MatrixAPI::CreateOrthographic
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateOrthographic(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept
		{
			return SetIdentityMatrix();
			// TO DO
		}

		// \copydoc NuMath::MatrixAPi::Decompose
		//NU_FORCEINLINE void Decompose(const NuMat4& m, NuVec4& translation, NuVec4& rotation, NuVec4& scale) noexcept
		//{
			// TO DO
		//}

		// \copydoc NuMath::MatrixAPI::Equal
		[[nodiscard]] static NU_FORCEINLINE bool Equal(const NuMat4& a, const NuMat4& b) noexcept
		{
			return Equal(a.cols[0], b.cols[0]) &&
				Equal(a.cols[1], b.cols[1]) &&
				Equal(a.cols[2], b.cols[2]) &&
				Equal(a.cols[3], b.cols[3]);
		}

		// \copydoc NuMath::MatrixAPI::NearEqual
		[[nodiscard]] static NU_FORCEINLINE bool NearEqual(const NuMat4& a, const NuMat4& b, float epsilon) noexcept
		{
			return NearEqual(a.cols[0], b.cols[0], epsilon) &&
				NearEqual(a.cols[1], b.cols[1], epsilon) &&
				NearEqual(a.cols[2], b.cols[2], epsilon) &&
				NearEqual(a.cols[3], b.cols[3], epsilon);
		}

		// \copydoc NuMath::MatrixAPI::IsIdentity
		[[nodiscard]] static NU_FORCEINLINE bool IsIdentity(const NuMat4& m, float epsilon) noexcept
		{
			NuMat4 identity = SetIdentityMatrix();
			return NearEqual(m, identity, epsilon);
		}

		// \copydoc NuMath::MatrixAPI::GetColumn
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetColumn(const NuMat4& m, int col) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 4, "Column index out of bounds");
			return m.cols[col];
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetRow(const NuMat4& m, int row) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4, "Column index out of bounds");

			if (row == 0) return { m.cols[0].x, m.cols[1].x, m.cols[2].x, m.cols[3].x };
			if (row == 1) return { m.cols[0].y, m.cols[1].y, m.cols[2].y, m.cols[3].y };
			if (row == 2) return { m.cols[0].z, m.cols[1].z, m.cols[2].z, m.cols[3].z };

			return { m.cols[0].w, m.cols[1].w, m.cols[2].w, m.cols[3].w };
		}

		// \copydoc NuMath::MatrixAPI::SetColumn
		static NU_FORCEINLINE void SetColumn(NuMat4& m, int col, const NuVec4& vec) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 4, "Column index out of bounds");
			m.cols[col] = vec;
		}

		// \copydoc NuMath::MatrixAPI::SetRow
		static NU_FORCEINLINE void SetRow(NuMat4& m, int row, const NuVec4& vec) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4, "Column index out of bounds");
			if (row == 0) { m.cols[0].x = vec.x, m.cols[1].x = vec.y, m.cols[2].x = vec.z, m.cols[3].x = vec.w; }
			if (row == 1) { m.cols[0].y = vec.x, m.cols[1].y = vec.y, m.cols[2].y = vec.z, m.cols[3].y = vec.w; }
			if (row == 2) { m.cols[0].z = vec.x, m.cols[1].z = vec.y, m.cols[2].y = vec.z, m.cols[3].z = vec.w; }
			m.cols[0].w = vec.x, m.cols[1].w = vec.y, m.cols[2].w = vec.z, m.cols[3].w = vec.w;
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat4& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4 && col >= 0 && col < 4);

			const NuVec4& column = m.cols[col];
			if (row == 0) return column.x;
			if (row == 1) return column.y;
			if (row == 2) return column.z;

			return column.w;
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float& Access(NuMat4& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4 && col >= 0 && col < 4);

			NuVec4& column = m.cols[col];
			if (row == 0) return column.x;
			if (row == 1) return column.y;
			if (row == 2) return column.z;

			return column.w;
		}

		static NU_FORCEINLINE const float* Data(const NuMat4& m) noexcept
		{
			return &m.cols[0].x;
		}

		// =============================================
		// Matrix3x3
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix3x3
		[[nodiscard]] static NU_FORCEINLINE NuMat3 SetIdentityMatrix3x3() noexcept
		{
			NuMat3 result{};

			result.cols[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
			result.cols[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
			result.cols[2] = { 0.0f, 0.0f, 1.0f, 0.0f };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Add(const NuMat3& a, const NuMat3& b) noexcept
		{
			NuMat3 result{};

			result.cols[0] = Add(a.cols[0], b.cols[0]);
			result.cols[1] = Add(a.cols[1], b.cols[1]);
			result.cols[2] = Add(a.cols[2], b.cols[2]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Sub(const NuMat3& a, const NuMat3& b) noexcept
		{
			NuMat3 result{};

			result.cols[0] = Sub(a.cols[0], b.cols[0]);
			result.cols[1] = Sub(a.cols[1], b.cols[1]);
			result.cols[2] = Sub(a.cols[2], b.cols[2]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuMat3& m, const NuVec4& v) noexcept
		{
			NuVec4 r0 = Mul(m.cols[0], SetAll(v.x));
			NuVec4 r1 = Mul(m.cols[1], SetAll(v.y));
			NuVec4 r2 = Mul(m.cols[2], SetAll(v.z));

			return Add(Add(r0, r1), r2);
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Mul(const NuMat3& a, const NuMat3& b) noexcept
		{
			NuMat3 result{};

			result.cols[0] = Mul(a, b.cols[0]);
			result.cols[1] = Mul(a, b.cols[1]);
			result.cols[2] = Mul(a, b.cols[2]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat3 FromColumns(const NuVec4& c0, const NuVec4& c1, const NuVec4& c2) noexcept
		{
			NuMat3 result{};

			result.cols[0] = c0;
			result.cols[1] = c1;
			result.cols[2] = c2;

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat3 FromRows(const NuVec4& r0, const NuVec4& r1, const NuVec4& r2) noexcept
		{
			NuMat3 result{};

			result.cols[0] = { r0.x, r1.x, r2.x, 0.0f };
			result.cols[1] = { r0.y, r1.y, r2.y, 0.0f };
			result.cols[2] = { r0.z, r1.z, r2.z, 0.0f };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::GetColumn
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetColumn(const NuMat3& m, int col) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 3);
			return m.cols[col];
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetRow(const NuMat3& m, int row) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 3);
			if (row == 0) return { m.cols[0].x, m.cols[1].x, m.cols[2].x, 0.0f };
			if (row == 1) return { m.cols[0].y, m.cols[1].y, m.cols[2].y, 0.0f };
			return { m.cols[0].z, m.cols[1].z, m.cols[2].z, 0.0f };
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat3& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 3 && col >= 0 && col < 3);

			const NuVec4& column = m.cols[col];
			if (row == 0) return column.x;
			if (row == 1) return column.y;
			return column.z;
		}

		// =============================================
		// Matrix2x2
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix2x2
		[[nodiscard]] static NU_FORCEINLINE NuMat2 SetIdentityMatrix2x2() noexcept
		{
			NuMat2 result{};

			result.cols[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
			result.cols[1] = { 0.0f, 1.0f, 0.0f, 0.0f };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat2 Add(const NuMat2& a, const NuMat2& b) noexcept
		{
			NuMat2 result{};

			result.cols[0] = Add(a.cols[0], b.cols[0]);
			result.cols[1] = Add(a.cols[1], b.cols[1]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat2 Sub(const NuMat2& a, const NuMat2& b) noexcept
		{
			NuMat2 result{};

			result.cols[0] = Sub(a.cols[0], b.cols[0]);
			result.cols[1] = Sub(a.cols[1], b.cols[1]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuMat2& m, const NuVec4& v) noexcept
		{
			NuVec4 r0 = Mul(m.cols[0], SetAll(v.x));
			NuVec4 r1 = Mul(m.cols[1], SetAll(v.y));
			return Add(r0, r1);
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat2 Mul(const NuMat2& a, const NuMat2& b) noexcept
		{
			NuMat2 result{};

			result.cols[0] = Mul(a, b.cols[0]);
			result.cols[1] = Mul(a, b.cols[1]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat2 FromColumns(const NuVec4& c0, const NuVec4& c1) noexcept
		{
			NuMat2 result{};

			result.cols[0] = c0;
			result.cols[1] = c1;

			return result;
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuMat2 FromRows(const NuVec4& r0, const NuVec4& r1) noexcept
		{
			NuMat2 result{};

			result.cols[0] = { r0.x, r1.x, 0.0f, 0.0f };
			result.cols[1] = { r0.y, r1.y, 0.0f, 0.0f };

			return result;
		}

		// \copydoc NuMath::MatrixAPI::GetColumn
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetColumn(const NuMat2& m, int col) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 2);
			return m.cols[col];
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetRow(const NuMat2& m, int row) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 2);
			if (row == 0) return { m.cols[0].x, m.cols[1].x, 0.0f, 0.0f };
			return { m.cols[0].y, m.cols[1].y, 0.0f, 0.0f };
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat2& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 2 && col >= 0 && col < 2, "");

			const NuVec4& column = m.cols[col];
			if (row == 0) return column.x;
			return column.y;
		}
	};
} // namespace NuMath::Scalar