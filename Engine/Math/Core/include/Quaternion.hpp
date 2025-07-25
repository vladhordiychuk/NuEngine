#pragma once

#include "../../SIMD/SIMDBackend.hpp"
#include "Vector4.hpp"
#include "Vector3.hpp"

#include <initializer_list>
#include <string>
#include <sstream>

namespace Engine::Math
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Vector4& vec);
		Quaternion(std::initializer_list<float> list);
		Quaternion(const Quaternion& other);

		float X() const;
		float Y() const;
		float Z() const;
		float W() const;

		const float* Data() const;
		const Simd::NuVec4& SimdData() const;

		Quaternion operator+(const Quaternion& other) const;
		Quaternion operator-(const Quaternion& other) const;
		Quaternion operator*(const Quaternion& other) const;
		Quaternion operator/(const Quaternion& other) const;
		Quaternion operator*(float scalar) const;
		Quaternion operator/(float scalar) const;

		Quaternion& operator+=(const Quaternion& other);
		Quaternion& operator-=(const Quaternion& other);
		Quaternion& operator*=(const Quaternion& other);
		Quaternion& operator/=(const Quaternion& other);
		Quaternion& operator*=(float scalar);
		Quaternion& operator/=(float scalar);

		bool operator==(const Quaternion& other) const;
		bool operator!=(const Quaternion& other) const;

		float Length() const;
		float LengthSquared() const;
		Quaternion Normalize() const;
		Quaternion Inverse() const;
		float Dot(const Quaternion& other) const;

		Vector3 RotateVector(const Vector3& vec) const;

		static Quaternion Identity() noexcept;
		static Quaternion FromAxisAngle(const Vector3& vec, float angleRadians);
		static Quaternion FromEuler(float pitchRadians, float yawRadians, float rollRadians);

		std::string ToString() const;

	private:
		Simd::NuVec4 m_data;

		explicit Quaternion(Simd::NuVec4 vec);
	};
}
