// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Detail/SIMD/SIMDBackend.hpp>
#include <Math/Algebra/Vector/VectorAPI.hpp>
#include <Math/Algebra/Vector/Vector4.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Core/Types/Types.hpp>

#include <initializer_list>
#include <string>
#include <sstream>

namespace NuEngine::Math
{
	using NuFloat = MuEngine::Core::Types::NuFloat;
	using NuInt32 = NuEngine::Core::Types::NuInt32;
	using NuBool = NuEngine::Core::Types::NuBool;
	/*
	* 
	*/
	class Quaternion
	{
	public:
		/*
		* 
		*/
		Quaternion() noexcept
		{

		}

		/*
		* 
		*/
		Quaternion(NuFloat x, NuFloat y, NuFloat z, NuFloat w) noexcept;

		/*
		* 
		*/
		Quaternion(const Vector4& other) noexcept;

		/*
		* 
		*/
		Quaternion(std::initializer_list<NuFloat> list) noexcept;

		/*
		* 
		*/
		Quaternion(const Quaternion& other) noexcept;

		/*
		* 
		*/
		Quaternion(Quaternion&& other) noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat GetComponent(int) noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat X() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat Y() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat Z() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat W() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] const NuFloat* Data() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] const VectorAPI::NuVec4& SimdData() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion operator+(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion operator-(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion operator*(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion operator/(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion operator*(NuFloat scalar) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion operator/(NuFloat scalar) const noexcept;

		/*
		* 
		*/
		Quaternion& operator+=(const Quaternion& other) noexcept;

		/*
		* 
		*/
		Quaternion& operator-=(const Quaternion& other) noexcept;

		/*
		* 
		*/
		Quaternion& operator*=(const Quaternion& other) noexcept;

		/*
		* 
		*/
		Quaternion& operator/=(const Quaternion& other) noexcept;

		/*
		* 
		*/
		Quaternion& operator*=(NuFloat scalar) noexcept;

		/*
		* 
		*/
		Quaternion& operator/=(NuFloat scalar) noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuBool operator==(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuBool operator!=(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat operator[](NuInt32 index) const noexcept;

		/*
		* 
		*/
		NuFloat& operator[](NuInt32) noexcept;

		/*
		* 
		*/
		Quaternion& operator=(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		Quaternion& operator=(Quaternion&& other) noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat Length() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat LengthSquared() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion Normalize() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Quaternion Inverse() const noexcept;

		/*
		* 
		*/
		[[nodiscard]] NuFloat Dot(const Quaternion& other) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] Vector3 RotateVector(const Vector3&) const noexcept;

		/*
		* 
		*/
		[[nodiscard]] static Quaternion Identity() noexcept;

		/*
		* 
		*/
		[[nodiscard]] static Quaternion FromAxisAngle(const Vector3& vec, NuFloat angleRadians) noexcept;

		/*
		* 
		*/
		[[nodiscard]] static Quaternion FromEuler(NuFloat pitchRadians, NuFloat yawRadians, NuFloat rollRadians) noexcept;

		/*
		* 
		*/
		[[nodiscard]] std::string ToString() const;

	private:
		/*
		* 
		*/
		VectorAPI::NuVec4 m_data;

		/*
		* 
		*/
		explicit Quaternion(VectorAPI::NuVec4 vec) : m_data(vec) {};
	};
}
