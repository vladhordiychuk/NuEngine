#pragma once

#include <smmintrin.h>
#include <cassert>
#include <cmath>

namespace Engine::Math::Simd_SSE
{
	// =============================================
	// Common types
	// =============================================

	using NuVec4 = __m128;

	struct NuMat4
	{
		__m128 cols[4];
	};

	struct NuMat3
	{
		__m128 cols[3];
	};

	// =============================================
	// Setters
	// =============================================

	__forceinline NuVec4 SetZero()
	{
		return _mm_setzero_ps();
	}

	__forceinline NuVec4 Set(float x, float y, float z = 0.0f, float w = 0.0f)
	{
		return _mm_set_ps(w, z, y, x);
	}

	__forceinline NuVec4 SetAll(float value)
	{
		return _mm_set1_ps(value);
	}

	// =============================================
	// Getters
	// =============================================

	__forceinline float GetX(NuVec4 v)
	{
		return _mm_cvtss_f32(v);
	}

	__forceinline float GetY(NuVec4 v)
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
	}

	__forceinline float GetZ(NuVec4 v)
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
	}

	__forceinline float GetW(NuVec4 v)
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
	}

	// =============================================
	// Basic Arithmetic
	// =============================================

	__forceinline NuVec4 Add(NuVec4 a, NuVec4 b) { return _mm_add_ps(a, b); }
	__forceinline NuVec4 Sub(NuVec4 a, NuVec4 b) { return _mm_sub_ps(a, b); }
	__forceinline NuVec4 Mul(NuVec4 a, NuVec4 b) { return _mm_mul_ps(a, b); }
	__forceinline NuVec4 Div(NuVec4 a, NuVec4 b) { return _mm_div_ps(a, b); }

	__forceinline NuVec4 Min(NuVec4 a, NuVec4 b) { return _mm_min_ps(a, b); }
	__forceinline NuVec4 Max(NuVec4 a, NuVec4 b) { return _mm_max_ps(a, b); }

	__forceinline bool Equal(NuVec4 a, NuVec4 b)
	{
		return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xF;
	}

	// =============================================
	// Special Ops
	// =============================================

	inline NuVec4 Abs(NuVec4 v)
	{
		const NuVec4 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
		return _mm_and_ps(v, mask);
	}

	__forceinline float SqrtScalar(float value)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value)));
	}

	inline float HorizontalAdd4(NuVec4 v)
	{
		NuVec4 shuf = _mm_movehdup_ps(v);
		NuVec4 sums = _mm_add_ps(v, shuf);       
		shuf = _mm_movehl_ps(shuf, sums); 
		sums = _mm_add_ss(sums, shuf);   
		return _mm_cvtss_f32(sums);
	}

	inline float HorizontalAdd3(NuVec4 v)
	{
		NuVec4 shuf1 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3)); 
		NuVec4 shuf2 = _mm_movehl_ps(shuf1, shuf1); 
		NuVec4 sum = _mm_add_ps(v, shuf2); 
		sum = _mm_add_ss(sum, _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(1, 1, 1, 1)));
		return _mm_cvtss_f32(sum); 
	}

	inline float HorizontalAdd2(NuVec4 v)
	{
		return GetX(v) + GetY(v);
	}

	inline NuVec4 Normalize2(NuVec4 v)
	{
		NuVec4 squared = Mul(v, v);
		float lenSq = HorizontalAdd2(squared);

		if (lenSq <= 0.0f)
			return SetZero();

		float invLen = 1.0f / SqrtScalar(lenSq);
		NuVec4 scale = Set(invLen, invLen, 1.0f, 1.0f);

		return Mul(v, scale);
	}

	inline NuVec4 Normalize3(NuVec4 v)
	{
		NuVec4 squared = Mul(v, v);
		float lengthSquared = HorizontalAdd3(squared);
		if (lengthSquared <= 0.0f)
			return SetZero();
		float invLength = 1.0f / SqrtScalar(lengthSquared);
		return Mul(v, SetAll(invLength));
	}

	inline NuVec4 Normalize4(NuVec4 v)
	{
		NuVec4 squared = Mul(v, v);
		float lengthSquared = HorizontalAdd4(squared);

		if (lengthSquared <= 0.0f)
			return SetZero();

		float invLength = 1.0f / SqrtScalar(lengthSquared);
		return Mul(v, SetAll(invLength));
	}

	inline NuVec4 Cross(NuVec4 a, NuVec4 b)
	{
		NuVec4 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
		NuVec4 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
		NuVec4 c = _mm_sub_ps(
			_mm_mul_ps(a, b_yzx),
			_mm_mul_ps(a_yzx, b)
		);
		return _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));
	}

	// =============================================
	// Extra Math Utilities
	// =============================================

	__forceinline float Dot3(NuVec4 a, NuVec4 b)
	{
		return HorizontalAdd3(Mul(a, b));
	}

	__forceinline float Length3(NuVec4 v)
	{
		return SqrtScalar(HorizontalAdd3(Mul(v, v)));
	}

	__forceinline NuVec4 Lerp(NuVec4 a, NuVec4 b, float t)
	{
		NuVec4 diff = Sub(b, a);
		return Add(a, Mul(diff, SetAll(t)));
	}

	// =============================================
	// Matricies
	// =============================================

	inline NuMat4 SetIdentityMatrix() {
		NuMat4 result{};
		result.cols[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
		result.cols[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
		result.cols[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
		result.cols[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
		return result;
	}

	inline NuMat4 Add(const NuMat4& a, const NuMat4& b)
	{
		NuMat4 result{};
		result.cols[0] = Add(a.cols[0], b.cols[0]);
		result.cols[1] = Add(a.cols[1], b.cols[1]);
		result.cols[2] = Add(a.cols[2], b.cols[2]);
		result.cols[3] = Add(a.cols[3], b.cols[3]);
		return result;
	}

	inline NuMat4 Sub(const NuMat4& a, const NuMat4& b)
	{
		NuMat4 result{};
		result.cols[0] = Sub(a.cols[0], b.cols[0]);
		result.cols[1] = Sub(a.cols[1], b.cols[1]);
		result.cols[2] = Sub(a.cols[2], b.cols[2]);
		result.cols[3] = Sub(a.cols[3], b.cols[3]);
		return result;
	}

	inline NuMat4 Multiply(const NuMat4& a, const NuMat4& b)
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

	inline NuVec4 Multiply(const NuMat4& mat, const NuVec4& v)
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

	inline NuMat4 Transpose(const NuMat4& mat)
	{
		NuMat4 result = mat;
		_MM_TRANSPOSE4_PS(result.cols[0], result.cols[1], result.cols[2], result.cols[3]);
		return result;
	}

	inline NuMat4 FromRows(const float* row0, const float* row1, const float* row2, const float* row3)
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

	inline NuMat4 FromColumns(const float* col0, const float* col1, const float* col2, const float* col3)
	{
		NuMat4 result{};
		result.cols[0] = _mm_loadu_ps(col0);
		result.cols[1] = _mm_loadu_ps(col1);
		result.cols[2] = _mm_loadu_ps(col2);
		result.cols[3] = _mm_loadu_ps(col3);
		return result;
	}

	float Determinant(const NuMat4& m)
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


	NuMat4 Inverse(const NuMat4& m)
	{
		NuVec4 Fac0, Fac1, Fac2, Fac3, Fac4, Fac5;

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

		__m128 Inv0 = _mm_mul_ps(Vec0, Fac1);
		__m128 Inv1 = _mm_mul_ps(Vec2, Fac0);

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

		__m128 Inv2 = _mm_mul_ps(Vec0, Fac0);
		__m128 Inv3 = _mm_mul_ps(Vec1, Fac2);

		Inv2 = _mm_sub_ps(Inv2, _mm_mul_ps(Vec1, Fac1));
		Inv3 = _mm_sub_ps(Inv3, _mm_mul_ps(Vec0, Fac3));

		__m128 SignA = _mm_set_ps(1.0f, -1.0f, 1.0f, -1.0f);
		__m128 SignB = _mm_set_ps(-1.0f, 1.0f, -1.0f, 1.0f);

		Inv0 = _mm_mul_ps(Inv0, SignA);
		Inv1 = _mm_mul_ps(Inv1, SignB);
		Inv2 = _mm_mul_ps(Inv2, SignA);
		Inv3 = _mm_mul_ps(Inv3, SignB);

		__m128 Row0 = _mm_mul_ps(m.cols[0], Inv0);
		__m128 Dot0 = _mm_hadd_ps(Row0, Row0);
		Dot0 = _mm_hadd_ps(Dot0, Dot0);

		__m128 Det = Dot0;
		__m128 RDet = _mm_div_ps(_mm_set1_ps(1.0f), Det);

		NuMat4 result{};
		result.cols[0] = _mm_mul_ps(Inv0, RDet);
		result.cols[1] = _mm_mul_ps(Inv1, RDet);
		result.cols[2] = _mm_mul_ps(Inv2, RDet);
		result.cols[3] = _mm_mul_ps(Inv3, RDet);

		return result;
	}

	inline NuMat4 CreateRotation(const NuVec4& quat)
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

	inline NuMat4 CreateScale(const NuVec4& scale)
	{
		NuMat4 result = SetIdentityMatrix();

		result.cols[0] = _mm_setr_ps(GetX(scale), 0.0f, 0.0f, 0.0f);
		result.cols[1] = _mm_setr_ps(0.0f, GetY(scale), 0.0f, 0.0f);
		result.cols[2] = _mm_setr_ps(0.0f, 0.0f, GetZ(scale), 0.0f);

		return result;
	}

	inline NuMat4 CreateLookAt(const NuVec4& eye, const NuVec4& target, const NuVec4& up)
	{
		// forward = normalize(target - eye)
		NuVec4 forward = Normalize(Sub(target, eye));
		// right = normalize(cross(up, forward))
		NuVec4 right = Normalize(Cross(up, forward));
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

	inline NuMat4 CreatePerspective(float fovY, float aspect, float nearZ, float farZ)
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

	inline NuMat4 CreateOrthographic(float left, float rigth, float bottom, float nearZ, float farZ)
	{
		return;
	}

	inline void Decompose(const NuMat4& mat, NuVec4& outTranslation, NuVec4& outRotationQuat, NuVec4& outScale)
	{
		outTranslation = _mm_setr_ps(GetX(mat.cols[3]), GetY(mat.cols[3]), GetZ(mat.cols[3]), 0.0f);

		float scaleX = SqrtScalar(HorizontalAdd3(Mul(mat.cols[0], mat.cols[0])));
		float scaleY = SqrtScalar(HorizontalAdd3(Mul(mat.cols[1], mat.cols[1])));
		float scaleZ = SqrtScalar(HorizontalAdd3(Mul(mat.cols[2], mat.cols[2])));
		outScale = _mm_setr_ps(scaleX, scaleY, scaleZ, 0.0f);

		NuVec4 normCol0 = Div(mat.cols[0], Set(scaleX));
		NuVec4 normCol1 = Div(mat.cols[1], Set(scaleY));
		NuVec4 normCol2 = Div(mat.cols[2], Set(scaleZ));

		float trace = GetX(normCol0) + GetY(normCol1) + GetZ(normCol2);
		float qw, qx, qy, qz;

		if (trace > 0.0f)
		{
			float s = 0.5f / sqrtf(trace + 1.0f);
			qw = 0.25f / s;
			qx = (GetY(normCol2) - GetZ(normCol1)) * s;
			qy = (GetZ(normCol0) - GetX(normCol2)) * s;
			qz = (GetX(normCol1) - GetY(normCol0)) * s;
		}
		else
		{
			if (GetX(normCol0) > GetY(normCol1) && GetX(normCol0) > GetZ(normCol2))
			{
				float s = 2.0f * sqrtf(1.0f + GetX(normCol0) - GetY(normCol1) - GetZ(normCol2));
				qw = (GetY(normCol2) - GetZ(normCol1)) / s;
				qx = 0.25f * s;
				qy = (GetY(normCol0) + GetX(normCol1)) / s;
				qz = (GetZ(normCol0) + GetX(normCol2)) / s;
			}
			else if (GetY(normCol1) > GetZ(normCol2))
			{
				float s = 2.0f * sqrtf(1.0f + GetY(normCol1) - GetX(normCol0) - GetZ(normCol2));
				qw = (GetZ(normCol0) - GetX(normCol2)) / s;
				qx = (GetY(normCol0) + GetX(normCol1)) / s;
				qy = 0.25f * s;
				qz = (GetZ(normCol1) + GetY(normCol2)) / s;
			}
			else
			{
				float s = 2.0f * sqrtf(1.0f + GetZ(normCol2) - GetX(normCol0) - GetY(normCol1));
				qw = (GetX(normCol1) - GetY(normCol0)) / s;
				qx = (GetZ(normCol0) + GetX(normCol2)) / s;
				qy = (GetZ(normCol1) + GetY(normCol2)) / s;
				qz = 0.25f * s;
			}
		}

		outRotationQuat = _mm_setr_ps(qx, qy, qz, qw);
	}

	__forceinline NuVec4 GetColumn(const NuMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return m.cols[index];
	}

	inline NuVec4 GetRow(const NuMat4& m, int index)
	{
		assert(index >= 0 && index < 4);

		alignas(16) float tmp[4][4]{};
		_mm_store_ps(&tmp[0][0], m.cols[0]);
		_mm_store_ps(&tmp[1][0], m.cols[1]);
		_mm_store_ps(&tmp[2][0], m.cols[2]);
		_mm_store_ps(&tmp[3][0], m.cols[3]);

		return Set(tmp[0][index], tmp[1][index], tmp[2][index], tmp[3][index]);
	}

	inline float Access(const NuMat4& m, int row, int col)
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		alignas(16) float tmp[4]{};
		_mm_store_ps(tmp, m.cols[col]);
		return tmp[row];
	}

	__forceinline const float* Data(const NuMat4& m)
	{
		return reinterpret_cast<const float*>(&m.cols[0]);
	}

	inline bool IsIdentity(const NuMat4& m, float epsilon = 0.0f)
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
}