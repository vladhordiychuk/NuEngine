// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/MathConstants.hpp>
#include <NuMath/Core/Math.hpp>

#include <bit>
#include <cmath>

namespace NuMath::Detail
{
	struct Scalar_Traits
	{
		// =============================================
		// Common types
		// =============================================

		struct alignas(16) NuVec4
		{
			float x, y, z, w;
		};

		using Register = NuVec4;

		static constexpr int Width = 4;

		struct alignas(16) NuMat4 
		{ 
			NuVec4 cols[4]; 
		};

		struct alignas(16) NuMat3 
		{ 
			NuVec4 cols[3]; 
		};

		struct alignas(16) NuMat2
		{ 
			NuVec4 cols[2];
		};

		struct alignas(16) NuTransform
		{
			NuVec4 position;
			NuVec4 rotation;
			NuVec4 scale;
		};

		// =============================================
		// Vectors
		// =============================================

		// \copydoc NuMath::VectorAPI::Load
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Load(const NuVecStorage4& vec) noexcept
		{
			return { vec.x, vec.y, vec.z, vec.w };
		}

		// \copydoc NuMath::VectorAPI::Load
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Load(const float* ptr) noexcept
		{
			return { ptr[0], ptr[1], ptr[2], ptr[3] };
		}

		// \copydoc NuMath::VectorAPI::Store
		static NU_FORCEINLINE void Store(NuVecStorage4& vec, NuVec4 val) noexcept
		{
			vec.x = val.x; vec.y = val.y; vec.z = val.z; vec.w = val.w;
		}

		// \copydoc NuMath::VectorAPI::Stream
		static NU_FORCEINLINE void Stream(float* ptr, NuVec4 val) noexcept
		{
			ptr[0] = val.x; ptr[1] = val.y; ptr[2] = val.z; ptr[3] = val.w;
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
			return { (a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y, (a.z < b.z) ? a.z : b.z, (a.w < b.w) ? a.w : b.w };
		}

		// \copydoc NuMath::VectorAPI::Max
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Max(const NuVec4& a, const NuVec4& b) noexcept
		{
			return { (a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y, (a.z > b.z) ? a.z : b.z, (a.w > b.w) ? a.w : b.w };
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
		[[nodiscard]] static NU_FORCEINLINE bool NearEqual(const NuVec4& a, const NuVec4& b, float epsilon = NuMath::EPSILON) noexcept
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

		// \copydoc NuMath::VectorAPI::InvSqrtFast
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
			NU_MATH_ASSERT(lenSq > 1e-8f, "Cannot normalize vector with near-zero length!");
			float invLen = InvSqrtFast(lenSq);
			return { v.x * invLen, v.y * invLen, v.z, v.w };
		}

		// \copydoc NuMath::VectorAPI::Normalize3
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize3(const NuVec4& v) noexcept
		{
			const float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
			NU_MATH_ASSERT(lenSq > 1e-8f, "Cannot normalize vector with near-zero length!");
			const float invLen = InvSqrtFast(lenSq);
			return { v.x * invLen, v.y * invLen, v.z * invLen, v.w };
		}

		// \copydoc NuMath::VectorAPI::Normalize4
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Normalize4(const NuVec4& v) noexcept
		{
			float lenSq = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
			NU_MATH_ASSERT(lenSq > 1e-8f, "Cannot normalize vector with near-zero length!");
			float invLen = InvSqrtFast(lenSq);
			return { v.x * invLen, v.y * invLen, v.z * invLen, v.w * invLen };
		}

		// \copydoc NuMath::VectorAPI::FastNormalize2
		[[nodiscard]] static NU_FORCEINLINE NuVec4 FastNormalize2(const NuVec4& v) noexcept
		{
			return Normalize2(v);
		}

		// \copydoc NuMath::VectorAPI::FastNormalize3
		[[nodiscard]] static NU_FORCEINLINE NuVec4 FastNormalize3(const NuVec4& v) noexcept
		{
			return Normalize3(v);
		}

		// \copydoc NuMath::VectorAPI::FastNormalize4
		[[nodiscard]] static NU_FORCEINLINE NuVec4 FastNormalize4(const NuVec4& v) noexcept
		{
			return Normalize4(v);
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

		// \copydoc NuMath::VectorAPI::Shuffle
		template <int I0, int I1, int I2, int I3>
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Shuffle(const NuVec4& v) noexcept
		{
			const float arr[] = { v.x, v.y, v.z, v.w };
			return { arr[I0], arr[I1], arr[I2], arr[I3] };
		}

		// =============================================
		// Quaternions
		// =============================================

		// \copydoc NuMath::QuaternionAPI::QuatIdentity
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatIdentity() noexcept
		{
			return { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		// \copydoc NuMath::QuaternionAPI::QuatMul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatMul(const NuVec4& a, const NuVec4& b) noexcept
		{
			return {
				a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
				a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
				a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
				a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
			};
		}

		// \copydoc NuMath::QuaternionAPI::QuatConjugate
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatConjugate(const NuVec4& q) noexcept
		{
			return { -q.x, -q.y, -q.z, q.w };
		}

		// \copydoc NuMath::QuaternionAPI::QuatInverse
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatInverse(const NuVec4& q) noexcept
		{
			float lenSq = Dot4(q, q);
			if (lenSq < EPSILON)
				return QuatIdentity();
			NuVec4 conj = QuatConjugate(q);
			float invLenSq = 1.0f / lenSq;
			return { conj.x * invLenSq, conj.y * invLenSq, conj.z * invLenSq, conj.w * invLenSq };
		}

		// \copydoc NuMath::QuaternionAPI::QuatRotateVector
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatRotateVector(const NuVec4& q, const NuVec4& v) noexcept
		{
			NuVec4 qv = { q.x, q.y, q.z, 0.0f };
			NuVec4 t = Mul(SetAll(2.0f), Cross(qv, v));
			NuVec4 wt = Mul(SetAll(q.w), t);
			NuVec4 cqt = Cross(qv, t);
			return Add(v, Add(wt, cqt));
		}

		// \copydoc NuMath::QuaternionAPI::QuatFromAxisAngle
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatFromAxisAngle(const NuVec4& axis, float angleRadians) noexcept
		{
			float half = angleRadians * 0.5f;
			float s = Sin(half);
			float c = Cos(half);
			return Normalize4({ axis.x * s, axis.y * s, axis.z * s, c });
		}

		// \copydoc NuMath::QuaternionAPI::QuatFromEuler
		[[nodiscard]] static NU_FORCEINLINE NuVec4 QuatFromEuler(float pitch, float yaw, float roll) noexcept
		{
			float p = pitch * 0.5f, y = yaw * 0.5f, r = roll * 0.5f;
			float cp = Cos(p), sp = Sin(p);
			float cy = Cos(y), sy = Sin(y);
			float cr = Cos(r), sr = Sin(r);
			return Normalize4({
				sr * cp * cy - cr * sp * sy,
				cr * sp * cy + sr * cp * sy,
				cr * cp * sy - sr * sp * cy,
				cr * cp * cy + sr * sp * sy
				});
		}

		// \copydoc NuMath::QuaternionAPI::Slerp
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Slerp(const NuVec4& a, NuVec4 b, float t) noexcept
		{
			float cosTheta = Dot4(a, b);
			if (cosTheta < 0.0f) { b = Neg(b); cosTheta = -cosTheta; }

			float scale0, scale1;
			if (cosTheta > 0.9995f)
			{
				scale0 = 1.0f - t;
				scale1 = t;
			}
			else
			{
				float theta = ACos(cosTheta);
				float sinTheta = Sin(theta);
				float inv = 1.0f / sinTheta;
				scale0 = Sin((1.0f - t) * theta) * inv;
				scale1 = Sin(t * theta) * inv;
			}
			return {
				scale0 * a.x + scale1 * b.x,
				scale0 * a.y + scale1 * b.y,
				scale0 * a.z + scale1 * b.z,
				scale0 * a.w + scale1 * b.w
			};
		}

		// =============================================
		// Transform
		// =============================================

		// \copydoc NuMath::TransformAPI::TransformIdentity
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformIdentity() noexcept
		{
			return { SetZero(), QuatIdentity(), Set(1.0f,1.0f,1.0f,0.0f) };
		}

		// \copydoc NuMath::TransformAPI::TransformCreate
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformCreate(NuVec4 position, NuVec4 rotation, NuVec4 scale) noexcept
		{
			return { position, rotation, scale };
		}

		// \copydoc NuMath::TransformAPI::TransformSetPosition
		static NU_FORCEINLINE void TransformSetPosition(NuTransform& t, NuVec4 p) noexcept
		{
			t.position = p;
		}

		// \copydoc NuMath::TransformAPI::TransformSetRotation
		static NU_FORCEINLINE void TransformSetRotation(NuTransform& t, NuVec4 r) noexcept
		{
			t.rotation = r;
		}

		// \copydoc NuMath::TransformAPI::TransformSetScale
		static NU_FORCEINLINE void TransformSetScale(NuTransform& t, NuVec4 s) noexcept
		{
			t.scale = s;
		}

		// \copydoc NuMath::TransformAPI::TransformGetPosition
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformGetPosition(const NuTransform& t) noexcept
		{
			return t.position;
		}

		// \copydoc NuMath::TransformAPI::TransformGetRotation
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformGetRotation(const NuTransform& t) noexcept
		{
			return t.rotation;
		}

		// \copydoc NuMath::TransformAPI::TransformGetScale
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformGetScale(const NuTransform& t) noexcept
		{
			return t.scale;
		}

		// \copydoc NuMath::TransformAPI::TransformToMatrix
		[[nodiscard]] static NU_FORCEINLINE NuMat4 TransformToMatrix(const NuTransform& t) noexcept
		{
			NuMat4 s = CreateScale(t.scale);
			NuMat4 r = CreateRotation(t.rotation);
			NuMat4 p = CreateTranslation(t.position);
			return Mul(p, Mul(r, s));
		}

		// \copydoc NuMath::TransformAPI::TransformToInverseMatrix
		[[nodiscard]] static NU_FORCEINLINE NuMat4 TransformToInverseMatrix(const NuTransform& t) noexcept
		{
			return TransformToMatrix(TransformInverse(t));
		}

		// \copydoc NuMath::TransformAPI::TransformInverse
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformInverse(const NuTransform& t) noexcept
		{
			NuTransform inv;
			inv.rotation = QuatInverse(t.rotation);
			inv.scale = Div(Set(1.0f, 1.0f, 1.0f, 0.0f), t.scale);
			NuVec4 scaledPos = Mul(t.position, inv.scale);
			inv.position = Neg(QuatRotateVector(inv.rotation, scaledPos));
			return inv;
		}

		// \copydoc NuMath::TransformAPI::TransformCombine
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformCombine(const NuTransform& parent, const NuTransform& child) noexcept
		{
			NuTransform result;
			result.scale = Mul(parent.scale, child.scale);
			result.rotation = QuatMul(parent.rotation, child.rotation);
			result.position = Add(parent.position,
				QuatRotateVector(parent.rotation, Mul(parent.scale, child.position)));
			return result;
		}

		// \copydoc NuMath::TransformAPI::TransformPoint
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformPoint(const NuTransform& t, NuVec4 point) noexcept
		{
			return Add(t.position, QuatRotateVector(t.rotation, Mul(t.scale, point)));
		}

		// \copydoc NuMath::TransformAPI::TransformDirection
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformDirection(const NuTransform& t, NuVec4 dir) noexcept
		{
			return QuatRotateVector(t.rotation, dir);
		}

		// \copydoc NuMath::TransformAPI::TransformGetForward
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformGetForward(const NuTransform& t) noexcept
		{
			return QuatRotateVector(t.rotation, Set(0.0f, 0.0f, -1.0f, 0.0f));
		}

		// \copydoc NuMath::TransformAPI::TransformGetUp
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformGetUp(const NuTransform& t) noexcept
		{
			return QuatRotateVector(t.rotation, Set(0.0f, 1.0f, 0.0f, 0.0f));
		}

		// \copydoc NuMath::TransformAPI::TransformGetRight
		[[nodiscard]] static NU_FORCEINLINE NuVec4 TransformGetRight(const NuTransform& t) noexcept
		{
			return QuatRotateVector(t.rotation, Set(1.0f, 0.0f, 0.0f, 0.0f));
		}

		// \copydoc NuMath::TransformAPI::TransformLerp
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformLerp(const NuTransform& a, const NuTransform& b, float t) noexcept
		{
			return { Lerp(a.position,b.position,t), Slerp(a.rotation,b.rotation,t), Lerp(a.scale,b.scale,t) };
		}

		// \copydoc NuMath::TransformAPI::TransformTranslation
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformTranslation(NuVec4 position) noexcept
		{
			return TransformCreate(position, QuatIdentity(), Set(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// \copydoc NuMath::TransformAPI::TransformRotation
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformRotation(NuVec4 rotation) noexcept
		{
			return TransformCreate(SetZero(), rotation, Set(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// \copydoc NuMath::TransformAPI::TransformScale
		[[nodiscard]] static NU_FORCEINLINE NuTransform TransformScale(NuVec4 scale) noexcept
		{
			return TransformCreate(SetZero(), QuatIdentity(), scale);
		}

		// \copydoc NuMath::TransformAPI::TransformEqual
		[[nodiscard]] static NU_FORCEINLINE bool TransformEqual(const NuTransform& a, const NuTransform& b) noexcept
		{
			return Equal(a.position, b.position) && Equal(a.rotation, b.rotation) && Equal(a.scale, b.scale);
		}

		// =============================================
		// Matrix4x4
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix
		[[nodiscard]] static NU_FORCEINLINE NuMat4 SetIdentityMatrix() noexcept
		{
			return { { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} } };
		}

		// \copydoc NuMath::MatrixAPI::Transpose
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Transpose(const NuMat4& m) noexcept
		{
			return { {
				{ m.cols[0].x, m.cols[1].x, m.cols[2].x, m.cols[3].x },
				{ m.cols[0].y, m.cols[1].y, m.cols[2].y, m.cols[3].y },
				{ m.cols[0].z, m.cols[1].z, m.cols[2].z, m.cols[3].z },
				{ m.cols[0].w, m.cols[1].w, m.cols[2].w, m.cols[3].w }
			} };
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Add(const NuMat4& a, const NuMat4& b) noexcept
		{
			return { { Add(a.cols[0],b.cols[0]), Add(a.cols[1],b.cols[1]),
					   Add(a.cols[2],b.cols[2]), Add(a.cols[3],b.cols[3]) } };
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Sub(const NuMat4& a, const NuMat4& b) noexcept
		{
			return { { Sub(a.cols[0],b.cols[0]), Sub(a.cols[1],b.cols[1]),
					   Sub(a.cols[2],b.cols[2]), Sub(a.cols[3],b.cols[3]) } };
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuMat4& m, const NuVec4& v) noexcept
		{
			return Add(Add(Mul(m.cols[0], SetAll(v.x)), Mul(m.cols[1], SetAll(v.y))),
				Add(Mul(m.cols[2], SetAll(v.z)), Mul(m.cols[3], SetAll(v.w))));
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat4 Mul(const NuMat4& a, const NuMat4& b) noexcept
		{
			return { { Mul(a,b.cols[0]), Mul(a,b.cols[1]), Mul(a,b.cols[2]), Mul(a,b.cols[3]) } };
		}

		// \copydoc NuMath::MatrixAPI::FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat4 FromRows(const NuVec4& r0, const NuVec4& r1, const NuVec4& r2, const NuVec4& r3) noexcept
		{
			return { {
				{ r0.x, r1.x, r2.x, r3.x },
				{ r0.y, r1.y, r2.y, r3.y },
				{ r0.z, r1.z, r2.z, r3.z },
				{ r0.w, r1.w, r2.w, r3.w }
			} };
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat4 FromColumns(const NuVec4& c0, const NuVec4& c1, const NuVec4& c2, const NuVec4& c3) noexcept
		{
			return { { c0, c1, c2, c3 } };
		}

		// \copydoc NuMath::MatrixAPI::Determinant
		[[nodiscard]] static NU_FORCEINLINE float Determinant(const NuMat4& m) noexcept
		{
			float sf00 = m.cols[2].z * m.cols[3].w - m.cols[2].w * m.cols[3].z;
			float sf01 = m.cols[1].z * m.cols[3].w - m.cols[1].w * m.cols[3].z;
			float sf02 = m.cols[1].z * m.cols[2].w - m.cols[1].w * m.cols[2].z;
			float sf03 = m.cols[0].z * m.cols[3].w - m.cols[0].w * m.cols[3].z;
			float sf04 = m.cols[0].z * m.cols[2].w - m.cols[0].w * m.cols[2].z;
			float sf05 = m.cols[0].z * m.cols[1].w - m.cols[0].w * m.cols[1].z;

			float cof0 = +(m.cols[1].y * sf00 - m.cols[2].y * sf01 + m.cols[3].y * sf02);
			float cof1 = -(m.cols[0].y * sf00 - m.cols[2].y * sf03 + m.cols[3].y * sf04);
			float cof2 = +(m.cols[0].y * sf01 - m.cols[1].y * sf03 + m.cols[3].y * sf05);
			float cof3 = -(m.cols[0].y * sf02 - m.cols[1].y * sf04 + m.cols[2].y * sf05);

			return m.cols[0].x * cof0 + m.cols[1].x * cof1 + m.cols[2].x * cof2 + m.cols[3].x * cof3;
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
			return { {
				{ cof00 * invDet, cof01 * invDet, cof02 * invDet, cof03 * invDet },
				{ cof10 * invDet, cof11 * invDet, cof12 * invDet, cof13 * invDet },
				{ cof20 * invDet, cof21 * invDet, cof22 * invDet, cof23 * invDet },
				{ cof30 * invDet, cof31 * invDet, cof32 * invDet, cof33 * invDet }
			} };
		}

		// \copydoc NuMath::MatrixAPI::CreateTranslation
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateTranslation(const NuVec4& t) noexcept
		{
			NuMat4 m = SetIdentityMatrix();
			m.cols[3] = { t.x, t.y, t.z, 1.0f };
			return m;
		}

		// \copydoc NuMath::MatrixAPI::CreateScale
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateScale(const NuVec4& s) noexcept
		{
			NuMat4 m = SetIdentityMatrix();
			m.cols[0].x = s.x;
			m.cols[1].y = s.y;
			m.cols[2].z = s.z;
			return m;
		}

		// \copydoc NuMath::MatrixAPI::CreateRotation
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateRotation(const NuVec4& q) noexcept
		{
			float x = q.x, y = q.y, z = q.z, w = q.w;
			float xx = x * x, yy = y * y, zz = z * z;
			float xy = x * y, xz = x * z, yz = y * z, wx = w * x, wy = w * y, wz = w * z;

			return { {
				{ 1.0f - 2.0f * (yy + zz),  2.0f * (xy + wz),    2.0f * (xz - wy),    0.0f },
				{ 2.0f * (xy - wz),        1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx),  0.0f },
				{ 2.0f * (xz + wy),        2.0f * (yz - wx),    1.0f - 2.0f * (xx + yy), 0.0f },
				{ 0.0f,                0.0f,             0.0f,             1.0f }
			} };
		}

		// \copydoc NuMath::MatrixAPI::CreateLookAt
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateLookAt(const NuVec4& eye, const NuVec4& target, const NuVec4& up) noexcept
		{
			NuVec4 forward = Normalize3(Sub(eye, target));
			NuVec4 right = Normalize3(Cross(up, forward));
			NuVec4 trueUp = Cross(forward, right);

			return FromRows(
				{ right.x,   right.y,   right.z,   -Dot3(right,   eye) },
				{ trueUp.x,  trueUp.y,  trueUp.z,  -Dot3(trueUp,  eye) },
				{ forward.x, forward.y, forward.z, -Dot3(forward, eye) },
				{ 0.0f,      0.0f,      0.0f,       1.0f }
			);
		}

		// \copydoc NuMath::MatrixAPI::CreatePerspective
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreatePerspective(float fovY, float aspect, float nearZ, float farZ) noexcept
		{
			float f = 1.0f / std::tan(fovY * 0.5f);
			float nf = 1.0f / (nearZ - farZ);

			NuMat4 m{};
			m.cols[0] = { f / aspect, 0.0f, 0.0f,                        0.0f };
			m.cols[1] = { 0.0f,     f,    0.0f,                        0.0f };
			m.cols[2] = { 0.0f,  0.0f, (farZ + nearZ) * nf,               -1.0f };
			m.cols[3] = { 0.0f,  0.0f, (2.0f * farZ * nearZ) * nf,           0.0f };
			return m;
		}

		// \copydoc NuMath::MatrixAPI::CreateOrthographic
		[[nodiscard]] static NU_FORCEINLINE NuMat4 CreateOrthographic(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept
		{
			float rl = 1.0f / (right - left);
			float tb = 1.0f / (top - bottom);
			float fn = 1.0f / (farZ - nearZ);

			NuMat4 m{};
			m.cols[0] = { 2.0f * rl,              0.0f,              0.0f,        0.0f };
			m.cols[1] = { 0.0f,              2.0f * tb,              0.0f,        0.0f };
			m.cols[2] = { 0.0f,              0.0f,         -2.0f * fn,            0.0f };
			m.cols[3] = { -(right + left) * rl, -(top + bottom) * tb, -(farZ + nearZ) * fn, 1.0f };
			return m;
		}

		// \copydoc NuMath::MatrixAPI::Equal
		[[nodiscard]] static NU_FORCEINLINE bool Equal(const NuMat4& a, const NuMat4& b) noexcept
		{
			return Equal(a.cols[0], b.cols[0]) && Equal(a.cols[1], b.cols[1]) &&
				Equal(a.cols[2], b.cols[2]) && Equal(a.cols[3], b.cols[3]);
		}

		// \copydoc NuMath::MatrixAPI::NearEqual
		[[nodiscard]] static NU_FORCEINLINE bool NearEqual(const NuMat4& a, const NuMat4& b, float epsilon = NuMath::EPSILON) noexcept
		{
			return NearEqual(a.cols[0], b.cols[0], epsilon) && NearEqual(a.cols[1], b.cols[1], epsilon) &&
				NearEqual(a.cols[2], b.cols[2], epsilon) && NearEqual(a.cols[3], b.cols[3], epsilon);
		}

		// \copydoc NuMath::MatrixAPI::IsIdentity
		[[nodiscard]] static NU_FORCEINLINE bool IsIdentity(const NuMat4& m, float epsilon = NuMath::EPSILON) noexcept
		{
			return NearEqual(m, SetIdentityMatrix(), epsilon);
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
			NU_MATH_ASSERT(row >= 0 && row < 4, "Row index out of bounds");
			switch (row) 
			{
				case 0: 
					return { m.cols[0].x, m.cols[1].x, m.cols[2].x, m.cols[3].x };
				case 1:
					return { m.cols[0].y, m.cols[1].y, m.cols[2].y, m.cols[3].y };
				case 2: 
					return { m.cols[0].z, m.cols[1].z, m.cols[2].z, m.cols[3].z };
				default: 
					return { m.cols[0].w, m.cols[1].w, m.cols[2].w, m.cols[3].w };
			}
		}

		// \copydoc NuMath::MatrixAPI::SetColumn
		static NU_FORCEINLINE void SetColumn(NuMat4& m, int col, const NuVec4& v) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 4, "Column index out of bounds");
			m.cols[col] = v;
		}

		// \copydoc NuMath::MatrixAPI::SetRow
		static NU_FORCEINLINE void SetRow(NuMat4& m, int row, const NuVec4& v) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4, "Row index out of bounds");
			switch (row) 
			{
				case 0: 
					m.cols[0].x = v.x; m.cols[1].x = v.y; m.cols[2].x = v.z; m.cols[3].x = v.w; break;
				case 1: 
					m.cols[0].y = v.x; m.cols[1].y = v.y; m.cols[2].y = v.z; m.cols[3].y = v.w; break;
				case 2: 
					m.cols[0].z = v.x; m.cols[1].z = v.y; m.cols[2].z = v.z; m.cols[3].z = v.w; break;
				case 3: 
					m.cols[0].w = v.x; m.cols[1].w = v.y; m.cols[2].w = v.z; m.cols[3].w = v.w; break;
			}
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat4& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4 && col >= 0 && col < 4, "Index out of bounds");
			const NuVec4& c = m.cols[col];
			switch (row) { case 0: return c.x; case 1: return c.y; case 2: return c.z; default: return c.w; }
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float& Access(NuMat4& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 4 && col >= 0 && col < 4, "Index out of bounds");
			NuVec4& c = m.cols[col];
			switch (row) { case 0: return c.x; case 1: return c.y; case 2: return c.z; default: return c.w; }
		}

		// \copydoc NuMath::MatrixAPI::Data
		static NU_FORCEINLINE const float* Data(const NuMat4& m) noexcept { return &m.cols[0].x; }

		// =============================================
		// Matrix3x3
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix3x3
		[[nodiscard]] static NU_FORCEINLINE NuMat3 SetIdentityMatrix3x3() noexcept
		{
			return { { {1,0,0,0}, {0,1,0,0}, {0,0,1,0} } };
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Add(const NuMat3& a, const NuMat3& b) noexcept
		{
			return { { Add(a.cols[0],b.cols[0]), Add(a.cols[1],b.cols[1]), Add(a.cols[2],b.cols[2]) } };
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Sub(const NuMat3& a, const NuMat3& b) noexcept
		{
			return { { Sub(a.cols[0],b.cols[0]), Sub(a.cols[1],b.cols[1]), Sub(a.cols[2],b.cols[2]) } };
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuMat3& m, const NuVec4& v) noexcept
		{
			return Add(Add(Mul(m.cols[0], SetAll(v.x)), Mul(m.cols[1], SetAll(v.y))),
				Mul(m.cols[2], SetAll(v.z)));
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat3 Mul(const NuMat3& a, const NuMat3& b) noexcept
		{
			return { { Mul(a,b.cols[0]), Mul(a,b.cols[1]), Mul(a,b.cols[2]) } };
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat3 FromColumns(const NuVec4& c0, const NuVec4& c1, const NuVec4& c2) noexcept
		{
			return { { c0, c1, c2 } };
		}

		// \copydoc NuMath::MatrixAPI::FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat3 FromRows(const NuVec4& r0, const NuVec4& r1, const NuVec4& r2) noexcept
		{
			return { {
				{ r0.x, r1.x, r2.x, 0.0f },
				{ r0.y, r1.y, r2.y, 0.0f },
				{ r0.z, r1.z, r2.z, 0.0f }
			} };
		}

		// \copydoc NuMath::MatrixAPI::GetColumn
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetColumn(const NuMat3& m, int col) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 3, "Column index out of bounds");
			return m.cols[col];
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetRow(const NuMat3& m, int row) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 3, "Row index out of bounds");
			switch (row) {
			case 0: return { m.cols[0].x, m.cols[1].x, m.cols[2].x, 0.0f };
			case 1: return { m.cols[0].y, m.cols[1].y, m.cols[2].y, 0.0f };
			default: return { m.cols[0].z, m.cols[1].z, m.cols[2].z, 0.0f };
			}
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat3& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 3 && col >= 0 && col < 3, "Index out of bounds");
			const NuVec4& c = m.cols[col];
			switch (row) { case 0: return c.x; case 1: return c.y; default: return c.z; }
		}

		// =============================================
		// Matrix2x2
		// =============================================

		// \copydoc NuMath::MatrixAPI::SetIdentityMatrix2x2
		[[nodiscard]] static NU_FORCEINLINE NuMat2 SetIdentityMatrix2x2() noexcept
		{
			return { { {1,0,0,0}, {0,1,0,0} } };
		}

		// \copydoc NuMath::MatrixAPI::Add
		[[nodiscard]] static NU_FORCEINLINE NuMat2 Add(const NuMat2& a, const NuMat2& b) noexcept
		{
			return { { Add(a.cols[0],b.cols[0]), Add(a.cols[1],b.cols[1]) } };
		}

		// \copydoc NuMath::MatrixAPI::Sub
		[[nodiscard]] static NU_FORCEINLINE NuMat2 Sub(const NuMat2& a, const NuMat2& b) noexcept
		{
			return { { Sub(a.cols[0],b.cols[0]), Sub(a.cols[1],b.cols[1]) } };
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuVec4 Mul(const NuMat2& m, const NuVec4& v) noexcept
		{
			return Add(Mul(m.cols[0], SetAll(v.x)), Mul(m.cols[1], SetAll(v.y)));
		}

		// \copydoc NuMath::MatrixAPI::Mul
		[[nodiscard]] static NU_FORCEINLINE NuMat2 Mul(const NuMat2& a, const NuMat2& b) noexcept
		{
			return { { Mul(a,b.cols[0]), Mul(a,b.cols[1]) } };
		}

		// \copydoc NuMath::MatrixAPI::FromColumns
		[[nodiscard]] static NU_FORCEINLINE NuMat2 FromColumns(const NuVec4& c0, const NuVec4& c1) noexcept
		{
			return { { c0, c1 } };
		}

		// \copydoc NuMath::MatrixAPI::FromRows
		[[nodiscard]] static NU_FORCEINLINE NuMat2 FromRows(const NuVec4& r0, const NuVec4& r1) noexcept
		{
			return { { { r0.x, r1.x, 0.0f, 0.0f }, { r0.y, r1.y, 0.0f, 0.0f } } };
		}

		// \copydoc NuMath::MatrixAPI::GetColumn
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetColumn(const NuMat2& m, int col) noexcept
		{
			NU_MATH_ASSERT(col >= 0 && col < 2, "Column index out of bounds");
			return m.cols[col];
		}

		// \copydoc NuMath::MatrixAPI::GetRow
		[[nodiscard]] static NU_FORCEINLINE NuVec4 GetRow(const NuMat2& m, int row) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 2, "Row index out of bounds");
			if (row == 0) return { m.cols[0].x, m.cols[1].x, 0.0f, 0.0f };
			return { m.cols[0].y, m.cols[1].y, 0.0f, 0.0f };
		}

		// \copydoc NuMath::MatrixAPI::Access
		[[nodiscard]] static NU_FORCEINLINE float Access(const NuMat2& m, int row, int col) noexcept
		{
			NU_MATH_ASSERT(row >= 0 && row < 2 && col >= 0 && col < 2, "Index out of bounds");
			return (row == 0) ? m.cols[col].x : m.cols[col].y;
		}
	};
} // namespace NuMath::Detail