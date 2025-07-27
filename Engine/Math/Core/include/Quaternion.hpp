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
		Quaternion() noexcept = default;
		Quaternion(float x, float y, float z, float w) noexcept;
		Quaternion(const Vector4&) noexcept;
		Quaternion(std::initializer_list<float> list) noexcept;
		Quaternion(const Quaternion&) noexcept = default;
		Quaternion(Quaternion&&) noexcept = default;

		[[nodiscard]] float GetComponent(int) noexcept;
		[[nodiscard]] float X() const noexcept;
		[[nodiscard]] float Y() const noexcept;
		[[nodiscard]] float Z() const noexcept;
		[[nodiscard]] float W() const noexcept;

		[[nodiscard]] const float* Data() const noexcept;
		[[nodiscard]] const Simd::NuVec4& SimdData() const noexcept;

		[[nodiscard]] Quaternion operator+(const Quaternion&) const noexcept;
		[[nodiscard]] Quaternion operator-(const Quaternion&) const noexcept;
		[[nodiscard]] Quaternion operator*(const Quaternion&) const noexcept;
		[[nodiscard]] Quaternion operator/(const Quaternion&) const noexcept;
		[[nodiscard]] Quaternion operator*(float) const noexcept;
		[[nodiscard]] Quaternion operator/(float) const noexcept;

		Quaternion& operator+=(const Quaternion&) noexcept;
		Quaternion& operator-=(const Quaternion&) noexcept;
		Quaternion& operator*=(const Quaternion&) noexcept;
		Quaternion& operator/=(const Quaternion&) noexcept;
		Quaternion& operator*=(float) noexcept;
		Quaternion& operator/=(float) noexcept;

		[[nodiscard]] bool operator==(const Quaternion&) const noexcept;
		[[nodiscard]] bool operator!=(const Quaternion&) const noexcept;

		[[nodiscard]] float operator[](int) const noexcept;
		float& operator[](int) noexcept;
		Quaternion& operator=(const Quaternion&) const noexcept = default;
		Quaternion& operator=(Quaternion&&) noexcept = default;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;
		[[nodiscard]] Quaternion Normalize() const noexcept;
		[[nodiscard]] Quaternion Inverse() const noexcept;
		[[nodiscard]] float Dot(const Quaternion&) const noexcept;

		[[nodiscard]] Vector3 RotateVector(const Vector3&) const noexcept;

		[[nodiscard]] static Quaternion Identity() noexcept;
		[[nodiscard]] static Quaternion FromAxisAngle(const Vector3& vec, float angleRadians) noexcept;
		[[nodiscard]] static Quaternion FromEuler(float pitchRadians, float yawRadians, float rollRadians) noexcept;

		[[nodiscard]] std::string ToString() const;

	private:
		Simd::NuVec4 m_data;

		explicit Quaternion(Simd::NuVec4 vec);
	};
}
