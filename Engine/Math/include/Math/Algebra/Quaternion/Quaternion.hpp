// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Detail/SIMD/SIMDBackend.hpp>
#include <Math/Algebra/Vector/VectorAPI.hpp>
#include <Quaternion/QuaternionAPI.hpp>
#include <Math/Algebra/Vector/Vector4.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Core/Types/Types.hpp>

#include <initializer_list>
#include <string>
#include <sstream>

namespace NuEngine::Math
{
	using NuFloat = Core::Types::NuFloat;
	using NuInt32 = Core::Types::NuInt32;
	using NuBool = Core::Types::NuBool;

	/**
	 * 
	 */
	class Quaternion
	{
	public:
		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion() noexcept
			: m_data(VectorAPI::)
		{
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion(NuFloat x, NuFloat y, NuFloat z, NuFloat w) noexcept
			: m_data(VectorAPI::Set(x, y, z, w)
		{
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion(const Vector4& other) noexcept
		{
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion(std::initializer_list<NuFloat> list) noexcept
		{

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
		[[nodiscard]] NU_FORCEINLINE NuFloat GetComponent(int index) noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat X() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat Y() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat Z() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat W() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] const NU_FORCEINLINE NuFloat* Data() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] const NU_FORCEINLINE VectorAPI::NuVec4& SimdData() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator+(Quaternion other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator-(Quaternion other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator*(Quaternion other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator/(Quaternion other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator*(NuFloat scalar) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion operator/(NuFloat scalar) const noexcept
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator+=(Quaternion other) noexcept
		{
			m_data = VectorAPI::Add(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator-=(Quaternion other) noexcept
		{
			m_data = VectorAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator*=(Quaternion other) noexcept
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator/=(Quaternion other) noexcept
		{
			m_data = VectorAPI::Div(m_data, other.m_data);
			return *this;
		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator*=(NuFloat scalar) noexcept
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator/=(NuFloat scalar) noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuBool operator==(Quaternion other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuBool operator!=(const Quaternion& other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat operator[](NuInt32 index) const noexcept
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE NuFloat& operator[](NuInt32 index) noexcept
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator=(const Quaternion& other) const noexcept
		{

		}

		/**
		 * 
		 */
		NU_FORCEINLINE Quaternion& operator=(Quaternion&& other) noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat Length() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat LengthSquared() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion Normalize() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Quaternion Inverse() const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat Dot(Quaternion other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] NU_FORCEINLINE Vector3 RotateVector(Vector3 other) const noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion Identity() noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion FromAxisAngle(Vector3 vec, NuFloat angleRadians) noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] static NU_FORCEINLINE Quaternion FromEuler(NuFloat pitchRadians, NuFloat yawRadians, NuFloat rollRadians) noexcept
		{

		}

		/**
		 * 
		 */
		[[nodiscard]] std::string ToString() const
		{
			std::ostringstream oss;
			oss << "(" << X(), ", " << Y() << ", " << Z() << ", " << W() << ")";
			return oss.str();
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
		VectorAPI::NuVec4 m_data;

		/**
		 * 
		 */
		explicit Quaternion(VectorAPI::NuVec4 vec) 
			: m_data(vec) {};
	};
}
