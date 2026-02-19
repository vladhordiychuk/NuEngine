// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Algebra/Quaternion/QuaternionAPI.hpp>
#include <NuMath/Algebra/Vector/Vector4.hpp>
#include <NuMath/Algebra/Vector/Vector3.hpp>
#include <NuMath/Algebra/Vector/Vector2.hpp>
#include <NuMath/Core/Common.hpp>

#include <initializer_list>
#include <string>
#include <format>

namespace NuMath
{
	/**
	 * 
	 */
	class alignas(16) Quaternion
	{
	public:
		/**
		 * @brief 
		 * 
		 */
		NU_FORCEINLINE Quaternion() noexcept
			: m_data(QuaternionAPI::Identity())
		{
		}

		/**
		 * @brief
		 * 
		 */
		NU_FORCEINLINE Quaternion(float x, float y, float z, float w) noexcept
			: m_data(QuaternionAPI::Set(x, y, z, w))
		{
		}

		/**
		 * @brief
		 * 
		 */
		NU_FORCEINLINE Quaternion(const Vector4& vec) noexcept
			: m_data(vec.SimdData())
		{
		}

		/**
		 * @brief
		 *
		 */
		NU_FORCEINLINE Quaternion(const Vector3& vec) noexcept
			: m_data(vec.X(), vec.Y(), vec.Z(), 0.0f)
		{
		}

		/**
		 * @brief
		 *
		 */
		NU_FORCEINLINE Quaternion(const Vector2& vec) noexcept
			: m_data(vec.X(), vec.Y(), 0.0f, 0.0f)
		{
		}

		/**
		 * @brief
		 */
		NU_FORCEINLINE Quaternion(std::initializer_list<float> list) noexcept
		{
			NU_MATH_ASSERT(list.size() == 4, "Quaternion must contain exactly 4 elements!");
			auto it = list.begin();
			float x = *it++;
			float y = *it++;
			float z = *it++;
			float w = *it++;
			m_data = QuaternionAPI::Set(x, y, z, w);
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion(const Quaternion& other) noexcept
			: m_data(other.m_data)
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion(Quaternion&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		/**
		 *
		 */
		explicit NU_FORCEINLINE Quaternion(QuaternionAPI::NuQuat vec) noexcept
			: m_data(vec)
		{
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float X() const noexcept
		{
			return QuaternionAPI::GetX(m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float Y() const noexcept
		{
			return QuaternionAPI::GetY(m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float Z() const noexcept
		{
			return QuaternionAPI::GetZ(m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float W() const noexcept
		{
			return QuaternionAPI::GetW(m_data);
		}

		/**
		 *
		 */
		NU_FORCEINLINE void SetX(float x) noexcept
		{
			m_data = QuaternionAPI::SetX(m_data, x);
		}

		/**
		 *
		 */
		NU_FORCEINLINE void SetY(float y) noexcept
		{
			m_data = QuaternionAPI::SetY(m_data, y);
		}

		/**
		 *
		 */
		NU_FORCEINLINE void SetZ(float z) noexcept
		{
			m_data = QuaternionAPI::SetZ(m_data, z);
		}

		/**
		 *
		 */
		NU_FORCEINLINE void SetW(float w) noexcept
		{
			m_data = QuaternionAPI::SetW(m_data, w);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE const float* Data() const noexcept
		{
			return reinterpret_cast<const float*>(&m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] const NU_FORCEINLINE QuaternionAPI::NuQuat& SimdData() const noexcept
		{
			return m_data;
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator+(Quaternion other) const noexcept
		{
			return Quaternion(QuaternionAPI::Add(m_data, other.m_data));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator-(Quaternion other) const noexcept
		{
			return Quaternion(QuaternionAPI::Sub(m_data, other.m_data));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator*(Quaternion other) const noexcept
		{
			return Quaternion(QuaternionAPI::Mul(m_data, other.m_data));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator/(Quaternion other) const noexcept
		{
			return Quaternion(QuaternionAPI::Div(m_data, other.m_data));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator*(float scalar) const noexcept
		{
			return Quaternion(QuaternionAPI::Mul(m_data, QuaternionAPI::SetAll(scalar)));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator/(float scalar) const noexcept
		{
			return Quaternion(QuaternionAPI::Div(m_data, QuaternionAPI::SetAll(scalar)));
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator+=(Quaternion other) noexcept
		{
			m_data = QuaternionAPI::Add(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator-=(Quaternion other) noexcept
		{
			m_data = QuaternionAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator*=(Quaternion other) noexcept
		{
			m_data = QuaternionAPI::Mul(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator/=(Quaternion other) noexcept
		{
			m_data = QuaternionAPI::Div(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator*=(float scalar) noexcept
		{
			m_data = QuaternionAPI::Mul(m_data, QuaternionAPI::SetAll(scalar));
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator/=(float scalar) noexcept
		{
			m_data = QuaternionAPI::Div(m_data, QuaternionAPI::SetAll(scalar));
			return *this;
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator==(Quaternion other) const noexcept
		{
			return QuaternionAPI::Equal(m_data, other.m_data);
		}

		[[nodiscard]] NU_FORCEINLINE bool NearEqual(Quaternion other, float epsilon) const noexcept
		{
			return QuaternionAPI::NearEqual(m_data, other.m_data, epsilon);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator!=(const Quaternion& other) const noexcept
		{
			return !(*this == other);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float operator[](int index) const noexcept
		{
			NU_MATH_ASSERT(index >= 0 && index < 4, "Quaternion index out of bounds! Valid range: 0..3");
			return reinterpret_cast<const float*>(&m_data)[index];
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator=(const Quaternion& other) noexcept
		{
			m_data = other.m_data;
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator=(Quaternion&& other) noexcept
		{
			m_data = std::move(other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float Length() const noexcept
		{
			return QuaternionAPI::Length4(m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float LengthSquared() const noexcept
		{
			return QuaternionAPI::Dot4(m_data, m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion Normalize() const noexcept
		{
			return Quaternion(QuaternionAPI::Normalize4(m_data));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion Inverse() const noexcept
		{
			return Quaternion(QuaternionAPI::Inverse(m_data));
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE float Dot(Quaternion other) const noexcept
		{
			return QuaternionAPI::Dot4(m_data, other.m_data);
		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Vector3 RotateVector(Vector3 vec) const noexcept
		{
			return Vector3(QuaternionAPI::RotateVector(m_data, vec.SimdData()));
		}

		/**
		 * 
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion Identity() noexcept
		{
			return Quaternion(QuaternionAPI::Identity());
		}

		/**
		 * 
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion FromAxisAngle(Vector3 vec, float angleRadians) noexcept
		{
			return Quaternion(QuaternionAPI::FromAxisAngle(vec.SimdData(), angleRadians));
		}

		/**
		 * 
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion FromEuler(float pitchRadians, float yawRadians, float rollRadians) noexcept
		{
			return Quaternion(QuaternionAPI::FromEuler(pitchRadians, yawRadians, rollRadians));
		}

		/**
		 *
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion Slerp(Quaternion a, Quaternion b, float t) noexcept
		{
			return Quaternion(QuaternionAPI::Slerp(a.m_data, b.m_data, t));
		}

		/**
		 * 
		 */
		[[nodiscard]] std::string ToString() const
		{
			return std::format("({}, {}, {}, {})", X(), Y(), Z(), W());
		}

		/**
		 * 
		 */
		friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
		{
			os << quat.ToString();
			return os;
		}

	private:
		/**
		 * 
		 */
		QuaternionAPI::NuQuat m_data;
	};
} // namespace NuMath