// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <smmintrin.h>
#include <cassert>
#include <cmath>

#include <Core/Types/Types.hpp>

namespace NuEngine::Math::Simd_SSE
{
	using NuFloat = NuEngine::Core::Types::NuFloat;
	using NuBool = NuEngine::Core::Types::NuBool;
	using NuInt32 = NuEngine::Core::Types::NuInt32;

	// =============================================
	// Common types
	// =============================================

	/*
	* @brief 4-component vector type with 16-byte alignment.
	*
	* Represents a SIMD-friendly vector with four float components (x, y, z, w).
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	using NuVec4 = __m128;

	/*
	* @brief 4x4 matrix type with 16-byte alignment.
	*
	* Represents a column-major 4x4 matrix stored in a 4D array.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuMat4
	{
		NuVec4 cols[4];
	};

	// =============================================
	// Vectors
	// =============================================

	/// \copydoc NuEngine::Math::VectorAPI::Set
	[[nodiscard]] NU_FORCEINLINE NuVec4 Set(NuFloat x, NuFloat y, NuFloat z = 0.0f, NuFloat w = 0.0f) noexcept
	{
		return _mm_set_ps(w, z, y, x);
	}

	/// \copydoc NuEngine::Math::VectorAPI::SetAll
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetAll(NuFloat scalar) noexcept
	{
		return _mm_set1_ps(scalar);
	}

	// \copydoc NuEngine::Math::VectorAPI::SetZero
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetZero() noexcept
	{
		return _mm_setzero_ps();
	}

	// \copydoc NuEngine::Math::VectorAPI::GetX
	[[nodiscard]] NU_FORCEINLINE NuFloat GetX(NuVec4 v) noexcept
	{
		return _mm_cvtss_f32(v);
	}

	// \copydoc NuEngine::Math::VectorAPI::GetY
	[[nodiscard]] NU_FORCEINLINE NuFloat GetY(NuVec4 v) noexcept
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
	}

	// \copydoc NuEngine::Math::VectorAPI::GetZ
	[[nodiscard]] NU_FORCEINLINE NuFloat GetZ(NuVec4 v) noexcept
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
	}

	// \copydoc NuEngine::Math::VectorAPI::GetW
	[[nodiscard]] NU_FORCEINLINE NuFloat GetW(NuVec4 v) noexcept
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
	}

	// \copydoc NuEngine::Math::VectorAPI::Add
	[[nodiscard]] NU_FORCEINLINE NuVec4 Add(NuVec4 a, NuVec4 b) noexcept
	{
		return _mm_add_ps(a, b);
	}

	// \copydoc NuEngine::Math::VectorAPI::Sub
	[[nodiscard]] NU_FORCEINLINE NuVec4 Sub(NuVec4 a, NuVec4 b) noexcept
	{
		return _mm_sub_ps(a, b);
	}

	// \copydoc NuEngine::Math::VectorAPI::Mul
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(NuVec4 a, NuVec4 b) noexcept
	{
		return _mm_mul_ps(a, b);
	}

	// \copydoc NuEngine::Math::VectorAPI::Div
	[[nodiscard]] NU_FORCEINLINE NuVec4 Div(NuVec4 a, NuVec4 b) noexcept
	{
		NuEngine::Core::Types::NuAssert(
			(std::fabs(GetX(b)) > NuFloat_EPSILON) &&
			(std::fabs(GetY(b)) > NuFloat_EPSILON) &&
			(std::fabs(GetZ(b)) > NuFloat_EPSILON) &&
			(std::fabs(GetW(b)) > NuFloat_EPSILON));
		return _mm_div_ps(a, b);
	}

	// \copydoc NuEngine::Math::VectorAPI::Neg
	[[nodiscard]] NU_FORCEINLINE NuVec4 Neg(NuVec4 v) noexcept
	{
		const NuVec4 sign_mask = _mm_set1_ps(-0.0f);
		return _mm_xor_ps(v, sign_mask);
	}

	// \copydoc NuEngine::Math::VectorAPI::Min
	[[nodiscard]] NU_FORCEINLINE NuVec4 Min(NuVec4 a, NuVec4 b) noexcept
	{
		return _mm_min_ps(a, b);
	}

	// \copydoc NuEngine::Math::VectorAPI::Max
	[[nodiscard]] NU_FORCEINLINE NuVec4 Max(NuVec4 a, NuVec4 b) noexcept
	{
		return _mm_max_ps(a, b);
	}

	// \copydoc NuEngine::Math::VectorAPI::Abs
	[[nodiscard]] NU_FORCEINLINE NuVec4 Abs(NuVec4 v) noexcept
	{
		const NuVec4 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
		return _mm_and_ps(v, mask);
	}

	// \copydoc NuEngine::Math::VectorAPI::Equal
	[[nodiscard]] NU_FORCEINLINE bool Equal(NuVec4 a, NuVec4 b)noexcept
	{
		return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xF;
	}

	// \copydoc NuEngine::Math::VectorAPI::NearEqual
	[[nodiscard]] NU_FORCEINLINE NuBool NearEqual(NuVec4 a, NuVec4 b, NuFloat epsilon) noexcept
	{
		NuVec4 diff = _mm_sub_ps(a, b);
		NuVec4 absDiff = Abs(diff);
		NuVec4 cmp = _mm_cmple_ps(absDiff, _mm_set1_ps(epsilon));
		return _mm_movemask_ps(cmp) == 0xF;
	}

	[[nodiscard]] NU_FORCEINLINE NuFloat SqrtScalar(NuFloat value) noexcept
	{
		NuVec4 v = _mm_set_ss(value);
		NuVec4 rsqrt = _mm_rsqrt_ss(v);
		NuVec4 sqrt = _mm_mul_ss(v, rsqrt);
		return _mm_cvtss_f32(sqrt);
	}

	// \copydoc NuEngine::Math::VectorAPI::HorizontalAdd4
	[[nodiscard]] NU_FORCEINLINE NuFloat HorizontalAdd4(NuVec4 v) noexcept
	{
		NuVec4 shuf = _mm_movehdup_ps(v);
		NuVec4 sums = _mm_add_ps(v, shuf);
		shuf = _mm_movehl_ps(shuf, sums);
		sums = _mm_add_ss(sums, shuf);
		return _mm_cvtss_f32(sums);
	}

	// \copydoc NuEngine::Math::VectorAPI::HorizontalAdd3
	[[nodiscard]] NU_FORCEINLINE NuFloat HorizontalAdd3(NuVec4 v) noexcept
	{
		NuVec4 shuf = _mm_movehdup_ps(v);
		NuVec4 sums = _mm_add_ps(v, shuf);
		return _mm_cvtss_f32(_mm_add_ss(sums, _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(2, 2, 2, 2))));
	}

	// \copydoc NuEngine::Math::VectorAPI::HorizontalAdd2
	[[nodiscard]] NU_FORCEINLINE NuFloat HorizontalAdd2(NuVec4 v) noexcept
	{
		NuVec4 shuf = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
		NuVec4 sum = _mm_add_ss(v, shuf);
		return _mm_cvtss_f32(sum);
	}

	// \copydoc NuEngine::Math::VectorAPI::Normalize2
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize2(NuVec4 v) noexcept
	{
		NuVec4 squared = Mul(v, v);
		NuFloat lenSq = HorizontalAdd2(squared);
		NuEngine::Core::Types::NuAssert(lenSq > 0.0f && "Cannot normalize zero vector!");
		NuFloat invLen = 1.0f / SqrtScalar(lenSq);
		NuVec4 scale = Set(invLen, invLen, 1.0f, 1.0f);

		return Mul(v, scale);
	}

	// \copydoc NuEngine::Math::VectorAPI::Normalize3
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize3(NuVec4 v) noexcept
	{
		NuVec4 squared = Mul(v, v);
		NuFloat lengthSquared = HorizontalAdd3(squared);
		NuEngine::Core::Types::NuAssert(lengthSquared > 0.0f && "Cannot normalize zero vector!");
		NuFloat invLength = 1.0f / SqrtScalar(lengthSquared);
		return Mul(v, SetAll(invLength));
	}

	// \copydoc NuEngine::Math::VectorAPI::Normalize4
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize4(NuVec4 v) noexcept
	{
		NuVec4 squared = Mul(v, v);
		NuFloat lengthSquared = HorizontalAdd4(squared);

		NuEngine::Core::Types::NuAssert(lengthSquared > 0.0f && "Cannot normalize zero vector!");

		NuFloat invLength = 1.0f / SqrtScalar(lengthSquared);
		return Mul(v, SetAll(invLength));
	}

	// \copydoc NuEngine::Math::VectorAPI::Cross
	[[nodiscard]] NU_FORCEINLINE NuVec4 Cross(NuVec4 a, NuVec4 b) noexcept
	{
		NuVec4 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
		NuVec4 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
		NuVec4 c = _mm_sub_ps(
			_mm_mul_ps(a, b_yzx),
			_mm_mul_ps(a_yzx, b)
		);
		return _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));
	}

	// \copydoc NuEngine::Math::VectorAPI::Dot2
	[[nodiscard]] NU_FORCEINLINE NuFloat Dot2(NuVec4 a, NuVec4 b) noexcept
	{
#if defined(__SSE4_1__)
		NuVec4 dp = _mm_dp_ps(a, b, 0x31);
		return _mm_cvtss_f32(dp);
#else
		NuVec4 mul = _mm_mul_ps(a, b);
		NuFloat sum = _mm_cvtss_f32(mul) + _mm_cvtss_f32(_mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 1, 1, 1)));
		return sum;
#endif
	}

	// \copydoc NuEngine::Math::VectorAPI::Dot3
	[[nodiscard]] NU_FORCEINLINE NuFloat Dot3(NuVec4 a, NuVec4 b) noexcept
	{
#if defined(__SSE4_1__)
		NuVec4 dp = _mm_dp_ps(a, b, 0x7F);
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

	// \copydoc NuEngine::Math::VectorAPI::Dot4
	[[nodiscard]] NU_FORCEINLINE NuFloat Dot4(NuVec4 a, NuVec4 b) noexcept
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

	// \copydoc NuEngine::Math::VectorAPI::Length2
	[[nodiscard]] NU_FORCEINLINE NuFloat Length2(NuVec4 v) noexcept
	{
		return SqrtScalar(Dot2(v, v));
	}

	// \copydoc NuEngine::Math::VectorAPI::Length3
	[[nodiscard]] NU_FORCEINLINE NuFloat Length3(NuVec4 v) noexcept
	{
		return SqrtScalar(Dot3(v, v));
	}

	// \copydoc NuEngine::Math::VectorAPI::Length4
	[[nodiscard]] NU_FORCEINLINE NuFloat Length4(NuVec4 v) noexcept
	{
		return SqrtScalar(Dot4(v, v));
	}

	// \copydoc NuEngine::Math::VectorAPI::Lerp
	[[nodiscard]] NU_FORCEINLINE NuVec4 Lerp(NuVec4 a, NuVec4 b, NuFloat t) noexcept
	{
		NuVec4 t_vec = _mm_set1_ps(t);
		return _mm_add_ps(a, _mm_mul_ps(_mm_sub_ps(b, a), t_vec));
	}

	// =============================================
	// Matricies
	// =============================================

	// \copydoc NuEngine::Math::MatrixAPI::SetIdentityMatrix
	NU_FORCEINLINE NuMat4 SetIdentityMatrix() {
		NuMat4 result{};
		result.cols[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
		result.cols[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
		result.cols[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
		result.cols[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
		return result;
	}

	//\copydoc NuEngine::Math::MatrixAPI::Add
	NU_FORCEINLINE NuMat4 Add(const NuMat4& a, const NuMat4& b)
	{
		NuMat4 result{};
		result.cols[0] = Add(a.cols[0], b.cols[0]);
		result.cols[1] = Add(a.cols[1], b.cols[1]);
		result.cols[2] = Add(a.cols[2], b.cols[2]);
		result.cols[3] = Add(a.cols[3], b.cols[3]);
		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Sub
	NU_FORCEINLINE NuMat4 Sub(const NuMat4& a, const NuMat4& b)
	{
		NuMat4 result{};
		result.cols[0] = Sub(a.cols[0], b.cols[0]);
		result.cols[1] = Sub(a.cols[1], b.cols[1]);
		result.cols[2] = Sub(a.cols[2], b.cols[2]);
		result.cols[3] = Sub(a.cols[3], b.cols[3]);
		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	NU_FORCEINLINE NuMat4 Mul(const NuMat4& a, const NuMat4& b)
	{
		NuMat4 result{};

		for (int i = 0; i < 4; ++i)
		{
			NuVec4 b_col = b.cols[i];

			NuVec4 e0 = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(0, 0, 0, 0)); // b[0][i]
			NuVec4 e1 = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(1, 1, 1, 1)); // b[1][i]
			NuVec4 e2 = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(2, 2, 2, 2)); // b[2][i]
			NuVec4 e3 = _mm_shuffle_ps(b_col, b_col, _MM_SHUFFLE(3, 3, 3, 3)); // b[3][i]

			NuVec4 r0 = _mm_mul_ps(a.cols[0], e0);
			NuVec4 r1 = _mm_mul_ps(a.cols[1], e1);
			NuVec4 r2 = _mm_mul_ps(a.cols[2], e2);
			NuVec4 r3 = _mm_mul_ps(a.cols[3], e3);

			NuVec4 sum = _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3));

			result.cols[i] = sum;
		}
		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Mul
	NU_FORCEINLINE NuVec4 Mul(const NuMat4& mat, const NuVec4& v)
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

	// \copydoc NuEngine::Math::MatrixAPI::Traspose
	NU_FORCEINLINE NuMat4 Transpose(const NuMat4& mat)
	{
		NuMat4 result = mat;
		_MM_TRANSPOSE4_PS(result.cols[0], result.cols[1], result.cols[2], result.cols[3]);
		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI:FromRows
	NU_FORCEINLINE NuMat4 FromRows(const float* row0, const float* row1, const float* row2, const float* row3)
	{
		NuMat4 result{};

		NuVec4 r0 = _mm_loadu_ps(row0); // [x0, y0, z0, w0]
		NuVec4 r1 = _mm_loadu_ps(row1); // [x1, y1, z1, w1]
		NuVec4 r2 = _mm_loadu_ps(row2); // [x2, y2, z2, w2]
		NuVec4 r3 = _mm_loadu_ps(row3); // [x3, y3, z3, w3]

		_MM_TRANSPOSE4_PS(r0, r1, r2, r3);

		result.cols[0] = r0; // [x0, x1, x2, x3]
		result.cols[1] = r1; // [y0, y1, y2, y3]
		result.cols[2] = r2; // [z0, z1, z2, z3]
		result.cols[3] = r3; // [w0, w1, w2, w3]

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::FromColumns
	NU_FORCEINLINE NuMat4 FromColumns(const NuFloat* col0, const NuFloat* col1, const NuFloat* col2, const NuFloat* col3)
	{
		NuMat4 result{};
		result.cols[0] = _mm_loadu_ps(col0);
		result.cols[1] = _mm_loadu_ps(col1);
		result.cols[2] = _mm_loadu_ps(col2);
		result.cols[3] = _mm_loadu_ps(col3);
		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Determinant
	NU_FORCEINLINE NuFloat Determinant(const NuMat4& m)
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

		NuFloat determinant;
		_mm_store_ss(&determinant, Temp);

		return determinant;
	}

	// \copydoc NuEngine::Math::MatrixAPI::Inverse
	NU_FORCEINLINE NuMat4 Inverse(const NuMat4& m)
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

	// \copydoc NuEngine::Math::MatrixAPI::CreateRotation
	NU_FORCEINLINE NuMat4 CreateRotation(const NuVec4& quat)
	{
		NuFloat x = GetX(quat);
		NuFloat y = GetY(quat);
		NuFloat z = GetZ(quat);
		NuFloat w = GetW(quat);

		NuFloat xx = x * x;
		NuFloat yy = y * y;
		NuFloat zz = z * z;
		NuFloat xy = x * y;
		NuFloat xz = x * z;
		NuFloat yz = y * z;
		NuFloat wx = w * x;
		NuFloat wy = w * y;
		NuFloat wz = w * z;

		NuMat4 result{};

		result.cols[0] = _mm_setr_ps(1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0.0f);
		result.cols[1] = _mm_setr_ps(2.0f * (xy - wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0.0f);
		result.cols[2] = _mm_setr_ps(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (xx + yy), 0.0f);
		result.cols[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateScale
	NU_FORCEINLINE NuMat4 CreateScale(const NuVec4& scale)
	{
		NuMat4 result = SetIdentityMatrix();

		result.cols[0] = _mm_setr_ps(GetX(scale), 0.0f, 0.0f, 0.0f);
		result.cols[1] = _mm_setr_ps(0.0f, GetY(scale), 0.0f, 0.0f);
		result.cols[2] = _mm_setr_ps(0.0f, 0.0f, GetZ(scale), 0.0f);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::CreateLookAt
	NU_FORCEINLINE NuMat4 CreateLookAt(const NuVec4& eye, const NuVec4& target, const NuVec4& up)
	{
		// forward = normalize(target - eye)
		NuVec4 forward = Normalize4(Sub(target, eye));
		// right = normalize(cross(up, forward))
		NuVec4 right = Normalize4(Cross(up, forward));
		// trueUp = cross(forward, right)
		NuVec4 trueUp = Cross(forward, right);

		NuMat4 result = SetIdentityMatrix();

		result.cols[0] = _mm_setr_ps(GetX(right), GetY(trueUp), GetX(forward), 0.0f);
		result.cols[1] = _mm_setr_ps(GetY(right), GetY(trueUp), GetY(forward), 0.0f);
		result.cols[2] = _mm_setr_ps(GetZ(right), GetZ(trueUp), GetZ(forward), 0.0f);

		result.cols[3] = _mm_setr_ps(
			-_mm_cvtss_f32(_mm_dp_ps(right, eye, 0x71)),  // -dot(right, eye)
			-_mm_cvtss_f32(_mm_dp_ps(trueUp, eye, 0x71)), // -dot(trueUp, eye)
			-_mm_cvtss_f32(_mm_dp_ps(forward, eye, 0x71)),// -dot(forward, eye)
			1.0f
		);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::CreatePerspective
	NU_FORCEINLINE NuMat4 CreatePerspective(NuFloat fovY, NuFloat aspect, NuFloat nearZ, NuFloat farZ)
	{
		NuFloat f = 1.0f / std::tan(fovY * 0.5f);
		NuFloat nf = 1.0f / (nearZ - farZ);

		NuMat4 result = {};

		result.cols[0] = _mm_setr_ps(f / aspect, 0.0f, 0.0f, 0.0f);
		result.cols[1] = _mm_setr_ps(0.0f, f, 0.0f, 0.0f);
		result.cols[2] = _mm_setr_ps(0.0f, 0.0f, (farZ + nearZ) * nf, -1.0f);
		result.cols[3] = _mm_setr_ps(0.0f, 0.0f, (2.0f * farZ * nearZ) * nf, 0.0f);

		return result;
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetColumn
	NU_FORCEINLINE NuVec4 GetColumn(const NuMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return m.cols[index];
	}

	// \copydoc NuEngine::Math::MatrixAPI::GetRow
	NU_FORCEINLINE NuVec4 GetRow(const NuMat4& m, int index)
	{
		assert(index >= 0 && index < 4);

		alignas(16) NuFloat tmp[4][4]{};
		_mm_store_ps(&tmp[0][0], m.cols[0]);
		_mm_store_ps(&tmp[1][0], m.cols[1]);
		_mm_store_ps(&tmp[2][0], m.cols[2]);
		_mm_store_ps(&tmp[3][0], m.cols[3]);

		return Set(tmp[0][index], tmp[1][index], tmp[2][index], tmp[3][index]);
	}

	// \copydoc NuEngine::Math::MatrixAPI::Access
	NU_FORCEINLINE NuFloat Access(const NuMat4& m, int row, int col)
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		alignas(16) NuFloat tmp[4]{};
		_mm_store_ps(tmp, m.cols[col]);
		return tmp[row];
	}

	// \copydoc NuEngine::Math::MatrixAPI::Data
	NU_FORCEINLINE const NuFloat* Data(const NuMat4& m)
	{
		return reinterpret_cast<const NuFloat*>(&m.cols[0]);
	}

	//copydoc NuEngine::Math::MatrixAPI::IsIdentity
	NU_FORCEINLINE bool IsIdentity(const NuMat4& m, NuFloat epsilon = 0.0f)
	{
		NuMat4 identity = SetIdentityMatrix();
		for (int i = 0; i < 4; ++i)
		{
			NuVec4 diff = _mm_sub_ps(m.cols[i], identity.cols[i]);

			NuVec4 absDiff = Abs(diff);

			alignas(16) NuFloat vals[4];
			_mm_store_ps(vals, absDiff);
			for (int j = 0; j < 4; ++j)
			{
				if (vals[j] > epsilon)
					return false;
			}
		}
		return true;
	}
}