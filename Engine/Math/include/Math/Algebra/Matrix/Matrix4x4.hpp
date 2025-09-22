// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

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
			: m_data(MatrixAPI::SetIdentityMatrix())
		{
		}

		/*
		*
		*/
		Matrix4x4(const Matrix4x4& other) noexcept
			: m_data(other.m_data)
		{
		}

		/*
		*
		*/
		Matrix4x4(Matrix4x4&& other) noexcept
			:m_data(std::move(other.m_data))
		{

		}

		/*
		*
		*/
		[[nodiscard]] static Matrix4x4 Identity() noexcept
		{
			return Matrix4x4(MatrixAPI::Identity());
		}

		/*
		*
		*/
		[[nodiscard]] static Matrix4x4 FromRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept
		{
			return Matrix4x4(MatrixAPI::FromRows(row0, row1, row2, row3));
		}

		/*
		*
		*/
		[[nodiscard]] static Matrix4x4 FromColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3) noexcept
		{
			return Matrix4x4(MatrixAPI::FromColumns(col0, col1, col2, col3));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 operator*(const Matrix4x4& other) const noexcept
		{
			return Matrix4x4(MatrixAPI::Mul(m_data, other.m_data));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Vector4 operator*(const Vector4& other) const noexcept
		{
			return Vector4(MatrixAPI::Mul(m_data, other.m_data));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 operator+(const Matrix4x4& other) const noexcept
		{
			return Matrix4x4(MatrixAPI::Add(m_data, other.m_data));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 operator-(const Matrix4x4& other) const noexcept
		{
			return Matrix4x4(MatrixAPI::Sub(m_data, other.m_data));
		}

		/*
		*
		*/
		NU_FORCEINLINE Matrix4x4& operator*=(const Matrix4x4& other) noexcept
		{
			m_data = MatrixAPI::Mul(m_data, other.m_data);
			return *this;
		}

		/*
		*
		*/
		Matrix4x4& operator+=(const Matrix4x4& other) noexcept
		{
			m_data = MatrixAPI::Add(m_data, other.m_data);
			return *this;
		}

		/*
		*
		*/
		Matrix4x4& operator-=(const Matrix4x4& other) noexcept
		{
			m_data = MatrixAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 Transpose() const noexcept
		{
			return Matrix4x4(MatrixAPI::Traspose(m_data));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE NuFloat Determinant() const noexcept
		{
			return MatrixAPI::Determinant(m_data);
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 Inverse() const noexcept
		{
			return Matrix4x4(MatrixAPI::Inverse(m_data));
		}

		/*
		*
		*/
		[[nodiscard]] static Matrix4x4 CreateTranslation(const Vector3& translation) noexcept
		{
			return Matrix4x4(MatrixAPI::CreateTranslation(translation));
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
		NU_FORCEINLINE void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const noexcept
		{

		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Vector4 GetColumn(NuInt32 index) const noexcept
		{
			return Vector4(MatrixAPI::GetColumn(m_data, index));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE Vector4 GetRow(NuInt32 index) const noexcept
		{
			return Vector4(MatrixAPI::GetRow(m_data, index));
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE const NuFloat* Data() const noexcept
		{
			return static_cast<const NuFloat*>(m_data);
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE NuFloat operator()(NuInt32 row, NuInt32 col) const noexcept
		{
			return MatrixAPI::Access(row, col);
		}

		/*
		*
		*/
		[[nodiscard]] NU_FORCEINLINE NuBool IsIdentity(NuFloat epsilon = 0.0f) const noexcept
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
		explicit Matrix4x4(const MatrixAPI::NuMat4& simdData) noexcept
			: m_data(simdData)
		{
		}
	};
}