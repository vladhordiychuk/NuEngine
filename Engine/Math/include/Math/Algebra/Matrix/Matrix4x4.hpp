// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Detail/SIMD/SIMDBackend.hpp>
#include <Math/Algebra/Matrix/MatrixAPI.hpp>
#include <Math/Algebra/Vector/Vector4.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Quaternion/Quaternion.hpp>
#include <Core/Types/Types.hpp>

#include <string>
#include <sstream>

namespace NuEngine::Math
{
	using NuInt32 = NuEngine::Core::Types::NuInt32;
	using NuFloat = NuEngine::Core::Types::NuFloat;
	using NuBool = NuEngine::Core::Types::NuBool;

	/*
	* 
	*/
	class alignas(16) Matrix4x4
	{
	public:
		/*
		* 
		*/
		Matrix4x4() noexcept
		{

		}

		/*
		* 
		*/
		Matrix4x4(const Matrix4x4& other) noexcept
		{

		}

		/*
		* 
		*/
		Matrix4x4(Matrix4x4&& other) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 Identity() noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 FromRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 FromColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Matrix4x4 operator*(const Matrix4x4&) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Vector4 operator*(const Vector4&) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Matrix4x4 operator+(const Matrix4x4&) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Matrix4x4 operator-(const Matrix4x4&) const noexcept
		{

		}

		/*
		* 
		*/
		Matrix4x4& operator*=(const Matrix4x4&) noexcept
		{

		}

		/*
		* 
		*/
		Matrix4x4& operator+=(const Matrix4x4&) noexcept
		{

		}

		/*
		* 
		*/
		Matrix4x4& operator-=(const Matrix4x4&) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Matrix4x4 Transpose() const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] NuFloat Determinant() const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Matrix4x4 Inverse() const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 CreateTranslation(const Vector3& translation) noexcept
		{

		}

		/*
		*
		*/
		[[nodiscard]] static Matrix4x4 CreateRotation(const Quaternion& rotation) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 CreateScale(const Vector3& scale) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 CreatePerspective(NuFloat fovY, NuFloat aspect, NuFloat nearZ, NuFloat farZ) noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] static Matrix4x4 CreateOrthographic(NuFloat left, NuFloat right, NuFloat bottom, NuFloat top, NuFloat nearZ, NuFloat farZ) noexcept
		{

		}

		/*
		* 
		*/
		void Decompose(Vector3& traslation, Quaternion& rotation, Vector3& scale) const noexcept
		{

		}


		/*
		* 
		*/
		[[nodiscard]] Vector4 GetColumn(NuInt32 index) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] Vector4 GetRow(NuInt32 index) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] const NuFloat* Data() const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] NuFloat operator()(NuInt32 row, NuInt32 col) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] NuBool IsIdentity(NuFloat epsilon = 0.0f) const noexcept
		{

		}

		/*
		* 
		*/
		[[nodiscard]] std::string ToString() const
		{

		}

	private:
		/*
		* 
		*/
		MatrixAPI::NuMat4 m_data;

		/*
		* 
		*/
		explicit Matrix4x4(const MatrixAPI::NuMat4& simdData) noexcept;
	};
}