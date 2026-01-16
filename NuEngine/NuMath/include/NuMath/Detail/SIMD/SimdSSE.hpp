// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/Constants.hpp>
#include <NuMath/Core/Math.hpp>

#include <immintrin.h>
#include <cassert>

namespace NuMath::Detail
{
	struct SSE_Traits
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
		using NuVec4 = __m128;

		using Register = NuVec4;

		static constexpr int Width = 4;

		/**
		 * @brief 4x4 matrix type with 16-byte alignment.
		 *
		 * Represents a column-major 4x4 matrix stored in a 4D array.
		 * The structure is aligned to 16 bytes for optimal SIMD operations.
		 */
		struct alignas(16) NuMat4
		{
			NuVec4 cols[4];
		};

		/**
		 * @brief 3x3 matrix type with 16-byte alignment.
		 *
		 * Represents a column-major 3x3 matrix stored in a 4D array.
		 * The structure is aligned to 16 bytes for optimal SIMD operations.
		 */
		struct alignas(16) NuMat3
		{
			NuVec4 cols[3];
		};

		/**
		 * @brief 2x2 matrix type with 16-byte alignment.
		 *
		 * Represents a column-major 2x2 matrix stored in a 4D array.
		 * The structure is aligned to 16 bytes for optimal SIMD operations.
		 */
		struct alignas(16) NuMat2
		{
			NuVec4 cols[2];
		};

		// =============================================
		// Vectors
		// =============================================

		// \copydoc NuMath::VectorAPI::Load
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Load(const NuVecStorage4& vec) noexcept
		{
			return _mm_load_ps(&vec.x);
		}

		[[nodiscard]] static NU_FORCEINLINE NuVec4 Load(const float* ptr) noexcept
		{
			return _mm_load_ps(ptr);
		}

		// \copydoc NuMath::VectorAPI::Store
		static NU_FORCEINLINE void Store(NuVecStorage4& vec, NuVec4 val) noexcept
		{
			_mm_store_ps(&vec.x, val);
		}

		// \copydoc NuMath::VectorAPI::Stream
		static NU_FORCEINLINE void Stream(float* ptr, NuVec4 val) noexcept
		{
			_mm_stream_ps(ptr, val);
		}

		// \copydoc NuEngine::Math::VectorAPI::Set
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Set(float x, float y, float z = 0.0f, float w = 0.0f) noexcept
		{
			return _mm_set_ps(w, z, y, x);
		}

		// \copydoc NuEngine::Math::VectorAPI::SetX
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetX(NuVec4 v, float x) noexcept
		{
			return _mm_move_ss(v, _mm_set_ss(x));
		}

		// \copydoc NuEngine::Math::VectorAPI::SetY
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetY(NuVec4 v, float y) noexcept
		{
#if defined(__SSE4_1__)
			return _mm_insert_ps(v, _mm_set_ss(y), 0x10);
#else
			NuVec4 yzw = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 2, 1, 0));
			yzw = _mm_move_ss(yzw, _mm_set_ss(y));
			return _mm_shuffle_ps(yzw, v, _MM_SHUFFLE(3, 2, 0, 0));
#endif
		}

		// \copydoc NuEngine::Math::VectorAPI::SetZ
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetZ(NuVec4 v, float z) noexcept
		{
#if defined(__SSE4_1__)
			return _mm_insert_ps(v, _mm_set_ss(z), 0x20);
#else
			NuVec4 xy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 0));
			NuVec4 zw = _mm_set_ss(z);
			zw = _mm_shuffle_ps(zw, v, _MM_SHUFFLE(3, 3, 0, 0));
			return _mm_shuffle_ps(xy, zw, _MM_SHUFFLE(2, 0, 1, 0));
#endif
		}

		// \copydoc NuEngine::Math::VectorAPI::SetW
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetW(NuVec4 v, float w) noexcept
		{
#if defined(__SSE4_1__)
			return _mm_insert_ps(v, _mm_set_ss(w), 0x30);
#else
			NuVec4 temp = _mm_set_ss(w);
			temp = _mm_shuffle_ps(v, temp, _MM_SHUFFLE(0, 0, 2, 0));
			return _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(2, 1, 1, 0));
#endif
		}

		// \copydoc NuEngine::Math::VectorAPI::SetAll
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetAll(float scalar) noexcept
		{
			return _mm_set1_ps(scalar);
		}

		// \copydoc NuEngine::Math::VectorAPI::SetZero
		[[nodiscard]] static NU_FORCEINLINE NuVec4 SetZero() noexcept
		{
			return _mm_setzero_ps();
		}

		// \copydoc NuEngine::Math::VectorAPI::GetX
		[[nodiscard]] static NU_FORCEINLINE float GetX(NuVec4 v) noexcept
		{
			return _mm_cvtss_f32(v);
		}

		// \copydoc NuEngine::Math::VectorAPI::GetY
		[[nodiscard]] static NU_FORCEINLINE float GetY(NuVec4 v) noexcept
		{
			return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
		}

		// \copydoc NuEngine::Math::VectorAPI::GetZ
		[[nodiscard]] static NU_FORCEINLINE float GetZ(NuVec4 v) noexcept
		{
			return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
		}

		// \copydoc NuEngine::Math::VectorAPI::GetW
		[[nodiscard]] static NU_FORCEINLINE float GetW(NuVec4 v) noexcept
		{
			return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
		}

		// \copydoc NuMath::VectorAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Add(NuVec4 a, NuVec4 b) noexcept
		{
			return _mm_add_ps(a, b);
		}

		// \copydoc NuMath::VectorAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Sub(NuVec4 a, NuVec4 b) noexcept
		{
			return _mm_sub_ps(a, b);
		}

		// \copydoc NuMath::VectorAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(NuVec4 a, NuVec4 b) noexcept
		{
			return _mm_mul_ps(a, b);
		}

		// \copydoc NuMath::VectorAPI::Neg
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Neg(NuVec4 v) noexcept
		{
			const NuVec4 sign_mask = _mm_set1_ps(-0.0f);
			return _mm_xor_ps(v, sign_mask);
		}

		// \copydoc NuMath::VectorAPI::Min
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Min(NuVec4 a, NuVec4 b) noexcept
		{
			return _mm_min_ps(a, b);
		}

		// \copydoc NuMath::VectorAPI::Max
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Max(NuVec4 a, NuVec4 b) noexcept
		{
			return _mm_max_ps(a, b);
		}

		// \copydoc NuMath::VectorAPI::Abs
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Abs(NuVec4 v) noexcept
		{
			const NuVec4 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
			return _mm_and_ps(v, mask);
		}

		// \copydoc NuMath::VectorAPI::Div
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Div(NuVec4 a, NuVec4 b) noexcept
		{
#if defined(NU_DEBUG) || !defined(NDEBUG)
			NuVec4 comparison = _mm_cmple_ps(Abs(b), SetAll(1.0e-6f));
			int mask = _mm_movemask_ps(comparison);
			NU_MATH_ASSERT(mask == 0, "Vector division by zero or near-zero value!");
#endif
			return _mm_div_ps(a, b);
		}

		// \copydoc NuMath::VectorAPI::Equal
		[[nodiscard]] static NU_FORCEINLINE bool Equal(NuVec4 a, NuVec4 b)noexcept
		{
			return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xF;
		}

		// \copydoc NuMath::VectorAPI::NearEqual
		[[nodiscard]] static NU_FORCEINLINE bool NearEqual(NuVec4 a, NuVec4 b, float epsilon = NuMath::EPSILON) noexcept
		{
			NuVec4 diff = _mm_sub_ps(a, b);
			NuVec4 absDiff = Abs(diff);
			NuVec4 cmp = _mm_cmple_ps(absDiff, _mm_set1_ps(epsilon));
			return _mm_movemask_ps(cmp) == 0xF;
		}

		[[nodiscard]] static NU_FORCEINLINE float SqrtScalar(float value) noexcept
		{
			return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value)));
		}

		// \copydoc NuMath::VectorAPI::HorizontalAdd4
		[[nodiscard]] static NU_FORCEINLINE float HorizontalAdd4(NuVec4 v) noexcept
		{
			NuVec4 shuf = _mm_movehdup_ps(v);
			NuVec4 sums = _mm_add_ps(v, shuf);
			shuf = _mm_movehl_ps(shuf, sums);
			sums = _mm_add_ss(sums, shuf);
			return _mm_cvtss_f32(sums);
		}

		// \copydoc NuMath::VectorAPI::HorizontalAdd3
		[[nodiscard]] static NU_FORCEINLINE float HorizontalAdd3(NuVec4 v) noexcept
		{
			NuVec4 shuf = _mm_movehdup_ps(v);
			NuVec4 sums = _mm_add_ps(v, shuf);
			return _mm_cvtss_f32(_mm_add_ss(sums, _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(2, 2, 2, 2))));
		}

		// \copydoc NuMath::VectorAPI::HorizontalAdd2
		[[nodiscard]] static NU_FORCEINLINE float HorizontalAdd2(NuVec4 v) noexcept
		{
			NuVec4 shuf = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
			NuVec4 sum = _mm_add_ss(v, shuf);
			return _mm_cvtss_f32(sum);
		}

		// \copydoc NuMath::VectorAPI::Normalize2
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize2(NuVec4 v) noexcept
		{
			NuVec4 squared = Mul(v, v);
			float lenSq = HorizontalAdd2(squared);
			NU_MATH_ASSERT(lenSq > 0.0f, "Cannot normalize zero vector!");
			float invLen = 1.0f / SqrtScalar(lenSq);
			NuVec4 scale = Set(invLen, invLen, 1.0f, 1.0f);

			return Mul(v, scale);
		}

		// \copydoc NuMath::VectorAPI::Normalize3
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize3(NuVec4 v) noexcept
		{
			NuVec4 squared = Mul(v, v);
			float lengthSquared = HorizontalAdd3(squared);
			NU_MATH_ASSERT(lengthSquared > 0.0f, "Cannot normalize zero vector!");
			float invLength = 1.0f / SqrtScalar(lengthSquared);
			return Mul(v, SetAll(invLength));
		}

		// \copydoc NuMath::VectorAPI::Normalize4
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize4(NuVec4 v) noexcept
		{
#if defined(__SSE4_1__)
			NuVec4 dot = _mm_dp_ps(v, v, 0xFF);
#else
			NuVec4 squared = _mm_mul_ps(v, v);
			NuVec4 shuf = _mm_movehdup_ps(squared);
			NuVec4 sums = _mm_add_ps(squared, shuf);
			shuf = _mm_movehl_ps(shuf, sums);
			NuVec4 dot = _mm_add_ss(sums, shuf);
			dot = _mm_shuffle_ps(dot, dot, 0);
#endif
			NuVec4 rsqrt = _mm_rsqrt_ps(dot);
			return _mm_mul_ps(v, rsqrt);
		}

		// \copydoc NuMath::VectorAPI::Cross
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Cross(NuVec4 a, NuVec4 b) noexcept
		{
			NuVec4 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
			NuVec4 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
			NuVec4 c = _mm_sub_ps(
				_mm_mul_ps(a, b_yzx),
				_mm_mul_ps(a_yzx, b)
			);
			return _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));
		}

		// \copydoc NuMath::VectorAPI::Dot2
		[[nodiscard]] static NU_FORCEINLINE float Dot2(NuVec4 a, NuVec4 b) noexcept
		{
#if defined(__SSE4_1__)
			NuVec4 dp = _mm_dp_ps(a, b, 0x31);
			return _mm_cvtss_f32(dp);
#else
			NuVec4 mul = _mm_mul_ps(a, b);
			float sum = _mm_cvtss_f32(mul) + _mm_cvtss_f32(_mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 1, 1, 1)));
			return sum;
#endif
		}

		// \copydoc NuMath::VectorAPI::Dot3
		[[nodiscard]] static NU_FORCEINLINE float Dot3(NuVec4 a, NuVec4 b) noexcept
		{
#if defined(__SSE4_1__)
			NuVec4 dp = _mm_dp_ps(a, b, 0x71);
			return _mm_cvtss_f32(dp);
#else
			NuVec4 mul = _mm_mul_ps(a, b);
			NuVec4 shuf1 = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(0, 0, 1, 1));
			NuVec4 sum1 = _mm_add_ss(mul, shuf1);
			NuVec4 shuf2 = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 2, 2, 2));
			NuVec4 sum2 = _mm_add_ss(sum1, shuf2);

			return _mm_cvtss_f32(sum2);
#endif
		}

		// \copydoc NuMath::VectorAPI::Dot4
		[[nodiscard]] static NU_FORCEINLINE float Dot4(NuVec4 a, NuVec4 b) noexcept
		{
#if defined(__SSE4_1__)
			NuVec4 dp = _mm_dp_ps(a, b, 0xF1);
			return _mm_cvtss_f32(dp);
#else
			NuVec4 mul = _mm_mul_ps(a, b);
			NuVec4 shuf = _mm_movehdup_ps(mul);
			NuVec4 sums = _mm_add_ps(mul, shuf);
			shuf = _mm_movehl_ps(shuf, sums);
			sums = _mm_add_ss(sums, shuf);
			return _mm_cvtss_f32(sums);
#endif
		}

		// \copydoc NuMath::VectorAPI::Length2
		[[nodiscard]] static NU_FORCEINLINE float Length2(NuVec4 v) noexcept
		{
			return SqrtScalar(Dot2(v, v));
		}

		// \copydoc NuMath::VectorAPI::Length3
		[[nodiscard]] static NU_FORCEINLINE float Length3(NuVec4 v) noexcept
		{
			return SqrtScalar(Dot3(v, v));
		}

		// \copydoc NuMath::VectorAPI::Length4
		[[nodiscard]] static NU_FORCEINLINE float Length4(NuVec4 v) noexcept
		{
			return SqrtScalar(Dot4(v, v));
		}

		// \copydoc NuMath::VectorAPI::Lerp
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Lerp(NuVec4 a, NuVec4 b, float t) noexcept
		{
			NuVec4 t_vec = _mm_set1_ps(t);
			return _mm_add_ps(a, _mm_mul_ps(_mm_sub_ps(b, a), t_vec));
		}

		// \copydoc NuMath::VectorAPI::Shuffle
		template <int I0, int I1, int I2, int I3>
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Shuffle(NuVec4 v) noexcept
		{
			static_assert(I0 >= 0 && I0 <= 3, "Shuffle index I0 out of bounds!");
			static_assert(I1 >= 0 && I1 <= 3, "Shuffle index I1 out of bounds!");
			static_assert(I2 >= 0 && I2 <= 3, "Shuffle index I2 out of bounds!");
			static_assert(I3 >= 0 && I3 <= 3, "Shuffle index I3 out of bounds!");

			return _mm_shuffle_ps(v, v, _MM_SHUFFLE(I3, I2, I1, I0));
		}

		// =============================================
		// Matrix4x4
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix
		[[nodiscard]] static NU_FORCEINLINE NuMat4 SetIdentityMatrix() noexcept
		{
			NuMat4 result{};

			result.cols[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
			result.cols[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
			result.cols[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
			result.cols[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

			return result;
		}

		//\copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Add(const NuMat4& a, const NuMat4& b) noexcept
		{
			NuMat4 result;

			result.cols[0] = Add(a.cols[0], b.cols[0]);
			result.cols[1] = Add(a.cols[1], b.cols[1]);
			result.cols[2] = Add(a.cols[2], b.cols[2]);
			result.cols[3] = Add(a.cols[3], b.cols[3]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Sub(NuMat4 a, NuMat4 b) noexcept
		{
			NuMat4 result;

			result.cols[0] = Sub(a.cols[0], b.cols[0]);
			result.cols[1] = Sub(a.cols[1], b.cols[1]);
			result.cols[2] = Sub(a.cols[2], b.cols[2]);
			result.cols[3] = Sub(a.cols[3], b.cols[3]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Traspose
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Transpose(NuMat4 mat) noexcept
		{
			NuMat4 result = mat;
			_MM_TRANSPOSE4_PS(result.cols[0], result.cols[1], result.cols[2], result.cols[3]);
			return result;
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Mul(const NuMat4& a, const NuMat4& b) noexcept
		{
			NuMat4 result;

			auto compute_col = [&](const NuVec4& b_col) -> NuVec4
				{
				const NuVec4 x = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(0, 0, 0, 0));
				const NuVec4 y = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(1, 1, 1, 1));
				const NuVec4 z = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(2, 2, 2, 2));
				const NuVec4 w = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(3, 3, 3, 3));

				NuVec4 r = _mm_mul_ps(a.cols[0], x);
				r = _mm_fmadd_ps(a.cols[1], y, r);
				r = _mm_fmadd_ps(a.cols[2], z, r);
				r = _mm_fmadd_ps(a.cols[3], w, r);
				return r;
				};

			result.cols[0] = compute_col(b.cols[0]);
			result.cols[1] = compute_col(b.cols[1]);
			result.cols[2] = compute_col(b.cols[2]);
			result.cols[3] = compute_col(b.cols[3]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(NuMat4 mat, NuVec4 v) noexcept
		{
			NuVec4 x = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
			NuVec4 y = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
			NuVec4 z = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
			NuVec4 w = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));

			NuVec4 r0 = _mm_mul_ps(mat.cols[0], x);
			NuVec4 r1 = _mm_mul_ps(mat.cols[1], y);
			NuVec4 r2 = _mm_mul_ps(mat.cols[2], z);
			NuVec4 r3 = _mm_mul_ps(mat.cols[3], w);

			NuVec4 result = _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3));
			return result;
		}

		// \copydoc NuMath::MatrixAPI:FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat4 FromRows(NuVec4 r0, NuVec4 r1, NuVec4 r2, NuVec4 r3) noexcept
		{
			NuMat4 result{};
			result.cols[0] = r0;
			result.cols[1] = r1;
			result.cols[2] = r2;
			result.cols[3] = r3;

			result = Transpose(result);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat4 FromColumns(NuVec4 c0, NuVec4 c1, NuVec4 c2, NuVec4 c3) noexcept
		{
			NuMat4 result{};
			result.cols[0] = c0;
			result.cols[1] = c1;
			result.cols[2] = c2;
			result.cols[3] = c3;
			return result;
		}

		// \copydoc NuMath::MatrixAPI::Determinant
		[[nodiscard]] static NU_FORCEINLINE float Determinant(NuMat4 m) noexcept
		{
			NuVec4 Vec0 = _mm_shuffle_ps(m.cols[2], m.cols[2], _MM_SHUFFLE(1, 0, 3, 2));
			NuVec4 Vec1 = _mm_shuffle_ps(m.cols[3], m.cols[3], _MM_SHUFFLE(2, 1, 0, 3));
			NuVec4 Vec2 = _mm_shuffle_ps(m.cols[2], m.cols[2], _MM_SHUFFLE(2, 1, 0, 3));
			NuVec4 Vec3 = _mm_shuffle_ps(m.cols[3], m.cols[3], _MM_SHUFFLE(1, 0, 3, 2));

			NuVec4 Fac0 = _mm_mul_ps(Vec0, Vec1);
			NuVec4 Fac1 = _mm_mul_ps(Vec2, Vec3);

			Fac0 = _mm_sub_ps(Fac0, _mm_mul_ps(Vec2, Vec1));

			NuVec4 Inv0 = _mm_mul_ps(m.cols[1], Fac0);

			NuVec4 SignA = _mm_set_ps(1.0f, -1.0f, 1.0f, -1.0f);
			Inv0 = _mm_mul_ps(Inv0, SignA);

			NuVec4 Row0 = _mm_mul_ps(m.cols[0], Inv0);

			NuVec4 Temp = _mm_hadd_ps(Row0, Row0);
			Temp = _mm_hadd_ps(Temp, Temp);

			float determinant;
			_mm_store_ss(&determinant, Temp);

			return determinant;
		}

		// \copydoc NuMath::MatrixAPI::Inverse
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Inverse(NuMat4 m) noexcept
		{
			NuVec4 Fac0, Fac1, Fac2, Fac3;

			NuVec4 Vec0 = _mm_shuffle_ps(m.cols[2], m.cols[2], _MM_SHUFFLE(1, 0, 3, 2));
			NuVec4 Vec1 = _mm_shuffle_ps(m.cols[3], m.cols[3], _MM_SHUFFLE(2, 1, 0, 3));
			NuVec4 Vec2 = _mm_shuffle_ps(m.cols[2], m.cols[2], _MM_SHUFFLE(2, 1, 0, 3));
			NuVec4 Vec3 = _mm_shuffle_ps(m.cols[3], m.cols[3], _MM_SHUFFLE(1, 0, 3, 2));

			Fac0 = _mm_mul_ps(Vec0, Vec1);
			Fac1 = _mm_mul_ps(Vec2, Vec3);

			Vec0 = _mm_shuffle_ps(m.cols[2], m.cols[2], _MM_SHUFFLE(0, 3, 2, 1));
			Vec1 = _mm_shuffle_ps(m.cols[3], m.cols[3], _MM_SHUFFLE(3, 2, 1, 0));
			Vec2 = _mm_shuffle_ps(m.cols[2], m.cols[2], _MM_SHUFFLE(3, 2, 1, 0));
			Vec3 = _mm_shuffle_ps(m.cols[3], m.cols[3], _MM_SHUFFLE(0, 3, 2, 1));

			Fac0 = _mm_sub_ps(_mm_mul_ps(Vec0, Vec1), Fac0);
			Fac1 = _mm_sub_ps(_mm_mul_ps(Vec2, Vec3), Fac1);

			Vec0 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(1, 0, 3, 2));
			Vec1 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(2, 1, 0, 3));
			Vec2 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(2, 1, 0, 3));
			Vec3 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(1, 0, 3, 2));

			NuVec4 Inv0 = _mm_mul_ps(Vec0, Fac1);
			NuVec4 Inv1 = _mm_mul_ps(Vec2, Fac0);

			Vec0 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(0, 3, 2, 1));
			Vec1 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(3, 2, 1, 0));
			Vec2 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(3, 2, 1, 0));
			Vec3 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(0, 3, 2, 1));

			Inv0 = _mm_sub_ps(Inv0, _mm_mul_ps(Vec0, Fac0));
			Inv1 = _mm_sub_ps(Inv1, _mm_mul_ps(Vec2, Fac1));

			Vec0 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(2, 1, 0, 3));
			Vec1 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(1, 0, 3, 2));
			Vec2 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(1, 0, 3, 2));
			Vec3 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(2, 1, 0, 3));

			Fac0 = _mm_mul_ps(Vec0, m.cols[3]);
			Fac1 = _mm_mul_ps(Vec1, m.cols[3]);
			Fac2 = _mm_mul_ps(Vec2, m.cols[3]);
			Fac3 = _mm_mul_ps(Vec3, m.cols[3]);

			Fac0 = _mm_sub_ps(Fac0, _mm_mul_ps(Vec1, m.cols[2]));
			Fac1 = _mm_sub_ps(Fac1, _mm_mul_ps(Vec0, m.cols[2]));
			Fac2 = _mm_sub_ps(Fac2, _mm_mul_ps(Vec3, m.cols[2]));
			Fac3 = _mm_sub_ps(Fac3, _mm_mul_ps(Vec2, m.cols[2]));

			Vec0 = _mm_shuffle_ps(m.cols[0], m.cols[0], _MM_SHUFFLE(3, 2, 1, 0));
			Vec1 = _mm_shuffle_ps(m.cols[1], m.cols[1], _MM_SHUFFLE(3, 2, 1, 0));

			NuVec4 Inv2 = _mm_mul_ps(Vec0, Fac0);
			NuVec4 Inv3 = _mm_mul_ps(Vec1, Fac2);

			Inv2 = _mm_sub_ps(Inv2, _mm_mul_ps(Vec1, Fac1));
			Inv3 = _mm_sub_ps(Inv3, _mm_mul_ps(Vec0, Fac3));

			NuVec4 SignA = _mm_set_ps(1.0f, -1.0f, 1.0f, -1.0f);
			NuVec4 SignB = _mm_set_ps(-1.0f, 1.0f, -1.0f, 1.0f);

			Inv0 = _mm_mul_ps(Inv0, SignA);
			Inv1 = _mm_mul_ps(Inv1, SignB);
			Inv2 = _mm_mul_ps(Inv2, SignA);
			Inv3 = _mm_mul_ps(Inv3, SignB);

			NuVec4 Row0 = _mm_mul_ps(m.cols[0], Inv0);
			NuVec4 Dot0 = _mm_hadd_ps(Row0, Row0);
			Dot0 = _mm_hadd_ps(Dot0, Dot0);

			NuVec4 Det = Dot0;
			NuVec4 RDet = _mm_div_ps(_mm_set1_ps(1.0f), Det);

			NuMat4 result{};
			result.cols[0] = _mm_mul_ps(Inv0, RDet);
			result.cols[1] = _mm_mul_ps(Inv1, RDet);
			result.cols[2] = _mm_mul_ps(Inv2, RDet);
			result.cols[3] = _mm_mul_ps(Inv3, RDet);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateRotation
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateRotation(NuVec4 quat) noexcept
		{
			float x = GetX(quat);
			float y = GetY(quat);
			float z = GetZ(quat);
			float w = GetW(quat);

			float xx = x * x;
			float yy = y * y;
			float zz = z * z;
			float xy = x * y;
			float xz = x * z;
			float yz = y * z;
			float wx = w * x;
			float wy = w * y;
			float wz = w * z;

			NuMat4 result{};

			result.cols[0] = _mm_setr_ps(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f);
			result.cols[1] = _mm_setr_ps(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0.0f);
			result.cols[2] = _mm_setr_ps(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy), 0.0f);
			result.cols[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateTranslation
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateTranslation(const NuVec4& v) noexcept
		{
			NuMat4 result = SetIdentityMatrix();

#if defined(__SSE4_1__)
			result.cols[3] = _mm_blend_ps(v, _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f), 0x08);
#else
			NuVec4 mask = _mm_castsi128_ps(_mm_set_epi32(0xFFFFFFFF, 0, 0, 0));
			NuVec4 w_one = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			result.cols[3] = _mm_or_ps(_mm_andnot_ps(mask, v), _mm_and_ps(mask, w_one));
#endif
			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateScale
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateScale(const NuVec4& scale) noexcept
		{
			NuMat4 result = SetIdentityMatrix();

			result.cols[0] = _mm_setr_ps(GetX(scale), 0.0f, 0.0f, 0.0f);
			result.cols[1] = _mm_setr_ps(0.0f, GetY(scale), 0.0f, 0.0f);
			result.cols[2] = _mm_setr_ps(0.0f, 0.0f, GetZ(scale), 0.0f);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateLookAt
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateLookAt(const NuVec4& eye, const NuVec4& target, const NuVec4& up) noexcept
		{
			NuVec4 forward = Normalize4(Sub(target, eye));
			NuVec4 right = Normalize4(Cross(up, forward));
			NuVec4 trueUp = Cross(forward, right);

			NuMat4 result = SetIdentityMatrix();

			result.cols[0] = _mm_setr_ps(GetX(right), GetY(trueUp), GetX(forward), 0.0f);
			result.cols[1] = _mm_setr_ps(GetY(right), GetY(trueUp), GetY(forward), 0.0f);
			result.cols[2] = _mm_setr_ps(GetZ(right), GetZ(trueUp), GetZ(forward), 0.0f);

			result.cols[3] = _mm_setr_ps(
				-_mm_cvtss_f32(_mm_dp_ps(right, eye, 0x71)),
				-_mm_cvtss_f32(_mm_dp_ps(trueUp, eye, 0x71)),
				-_mm_cvtss_f32(_mm_dp_ps(forward, eye, 0x71)),
				1.0f
			);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreatePerspective
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreatePerspective(float fovY, float aspect, float nearZ, float farZ) noexcept
		{
			float f = 1.0f / std::tan(fovY * 0.5f);
			float nf = 1.0f / (nearZ - farZ);

			NuMat4 result = {};

			result.cols[0] = _mm_setr_ps(f / aspect, 0.0f, 0.0f, 0.0f);
			result.cols[1] = _mm_setr_ps(0.0f, f, 0.0f, 0.0f);
			result.cols[2] = _mm_setr_ps(0.0f, 0.0f, (farZ + nearZ) * nf, -1.0f);
			result.cols[3] = _mm_setr_ps(0.0f, 0.0f, (2.0f * farZ * nearZ) * nf, 0.0f);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::CreateOrthographic
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateOrthographic(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept
		{
			return SetIdentityMatrix();
			// TO DO
		}

		// \copydoc NuMath::MatrixAPI::Decompose
		//NU_FORCEINLINE void Decompose() noexcept
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
		[[nodiscard]] static NU_FORCEINLINE bool NearEqual(const NuMat4& a, const NuMat4& b, float epsilon = NuMath::EPSILON) noexcept
		{
			return NearEqual(a.cols[0], b.cols[0], epsilon) &&
				NearEqual(a.cols[1], b.cols[1], epsilon) &&
				NearEqual(a.cols[2], b.cols[2], epsilon) &&
				NearEqual(a.cols[3], b.cols[3], epsilon);
		}

		// \copydoc NuMath::MatrixAPI::GetColumn
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetColumn(const NuMat4& m, int index) noexcept
		{
			NU_MATH_ASSERT(index >= 0 && index < 4, "Index out of bounds");
			return m.cols[index];
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetRow(const NuMat4& m, int index) noexcept
		{
			NU_MATH_ASSERT(index >= 0 && index < 4, "Index out of bounds");

			NuMat4 trasposedMatrix = Transpose(m);
			return trasposedMatrix.cols[index];
		}

		static NU_FORCEINLINE void SetColumn(NuMat4& m, int col, NuVec4 v) noexcept {
			NU_MATH_ASSERT(col >= 0 && col < 4, "Column index out of bounds");
			m.cols[col] = v;
		}

		static NU_FORCEINLINE void SetRow(NuMat4& m, int row, NuVec4 v) noexcept {
			NU_MATH_ASSERT(row >= 0 && row < 4, "Row index out of bounds");
			float* c0 = reinterpret_cast<float*>(&m.cols[0]);
			float* c1 = reinterpret_cast<float*>(&m.cols[1]);
			float* c2 = reinterpret_cast<float*>(&m.cols[2]);
			float* c3 = reinterpret_cast<float*>(&m.cols[3]);

			alignas(16) float val[4];
			_mm_store_ps(val, v);

			c0[row] = val[0];
			c1[row] = val[1];
			c2[row] = val[2];
			c3[row] = val[3];
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat4& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4 && col >= 0 && col < 4, "Index out of bounds");

			const NuVec4& column = m.cols[col];

			switch (row)
			{
			case 0: return GetX(column);
			case 1: return GetY(column);
			case 2: return GetZ(column);
			case 3: return GetW(column);
			default:
				return 0.0f;
			}
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float& Access(NuMat4& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4 && col >= 0 && col < 4, "Index out of bounds");

			float* data = reinterpret_cast<float*>(&m.cols[col]);

			return data[row];
		}

		// \copydoc NuMath::MatrixAPI::Data
		[[nodiscard]] static NU_FORCEINLINE const float* Data(const NuMat4& m) noexcept
		{
			return reinterpret_cast<const float*>(&m.cols[0]);
		}

		//copydoc NuMath::MatrixAPI::IsIdentity
		[[nodiscard]] static NU_FORCEINLINE bool IsIdentity(const NuMat4& m, float epsilon = NuMath::EPSILON) noexcept
		{
			NuMat4 identity = SetIdentityMatrix();
			for (int i = 0; i < 4; ++i)
			{
				NuVec4 diff = _mm_sub_ps(m.cols[i], identity.cols[i]);

				NuVec4 absDiff = Abs(diff);

				alignas(16) float vals[4];
				_mm_store_ps(vals, absDiff);
				for (int j = 0; j < 4; ++j)
				{
					if (vals[j] > epsilon)
						return false;
				}
			}
			return true;
		}

		// =============================================
		// Matrix3x3
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix3x3
		[[nodiscard]] static NU_FORCEINLINE NuMat3 SetIdentityMatrix3x3() noexcept
		{
			NuMat3 result{};

			result.cols[0] = _mm_setr_ps(1, 0, 0, 0);
			result.cols[1] = _mm_setr_ps(0, 1, 0, 0);
			result.cols[2] = _mm_setr_ps(0, 0, 1, 0);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Add(const NuMat3& a, const NuMat3& b) noexcept
		{
			NuMat3 result{};

			result.cols[0] = _mm_add_ps(a.cols[0], b.cols[0]);
			result.cols[1] = _mm_add_ps(a.cols[1], b.cols[1]);
			result.cols[2] = _mm_add_ps(a.cols[2], b.cols[2]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Sub(const NuMat3& a, const NuMat3& b) noexcept
		{
			NuMat3 result{};

			result.cols[0] = _mm_sub_ps(a.cols[0], b.cols[0]);
			result.cols[1] = _mm_sub_ps(a.cols[1], b.cols[1]);
			result.cols[2] = _mm_sub_ps(a.cols[2], b.cols[2]);

			return result;
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Mul(const NuMat3& a, const NuMat3& b) noexcept
		{
			NuMat3 result{};

			for (int i = 0; i < 3; ++i)
			{

			}
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat3 FromColumns(const NuVec4& col0, const NuVec4& col1, const NuVec4& col2) noexcept
		{
			NuMat3 result;

			result.cols[0] = col0;
			result.cols[1] = col1;
			result.cols[2] = col2;

			return result;
		}

		// \copydoc NuMath::MatrixAPI::FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat3 FromRows(const NuVec4& row0, const NuVec4& row1, const NuVec4& row2) noexcept
		{
			NuMat3 result;

			result.cols[0] = row0;
			result.cols[1] = row1;
			result.cols[2] = row2;

			return result; // TO DO
		}

		// =============================================
		// Matrix2x2
		// =============================================


	};
} // namespace NuMath::SSE