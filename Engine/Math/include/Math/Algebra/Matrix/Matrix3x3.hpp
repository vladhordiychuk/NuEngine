// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Algebra/Matrix/MatrixAPI.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>

#include <Core/Types/Types.hpp>

namespace NuEngine::Math
{
	using NuInt32 = Core::Types::NuInt32;
	using NuFloat = Core::Types::NuFloat;
	using NuBool = Core::Types::NuBool;

	/**
	 * @brief A 3x3 matrix aligned to 16-byte boundary for SIMD
	 */
	class alignas(16) Matrix3x3
	{
	public:
		/**
		 *
		 *
		 */
		NU_FORCEINLINE Matrix3x3() noexcept
			: m_data(MatrixAPI::SetIdentityMatrix())
		{
		}

		
		NU_FORCEINLINE Matrix3x3(std::initializer_list<NuFloat> list) noexcept
		{

		}

		/**
		 * @brief Constructs a matrix from 9 scalar values.
		 *
		 * Assumes the values are provided in row-major order.
		 * (m00, m01, m02, m03, ...)
		 *
		 * @param m00 Element at row 0, column 0.
		 * @param m01 Element at row 0, column 1.
		 * @param m02 Element at row 0, column 2.
		 * @param m10 Element at row 1, column 0.
		 * @param m11 Element at row 1, column 1.
		 * @param m12 Element at row 1, column 2.
		 * @param m20 Element at row 2, column 0.
		 * @param m21 Element at row 2, column 1.
		 * @param m22 Element at row 2, column 2.
		 */
		NU_FORCEINLINE Matrix3x3(NuFloat m00, NuFloat m01, NuFloat m02,
								 NuFloat m10, NuFloat m11, NuFloat m12,
								 NuFloat m20, NuFloat m21, NuFloat m22) noexcept
		{
			Vector3 row0(m00, m01, m02);
			Vector3 row1(m10, m11, m12);
			Vector3 row2(m20, m21, m22);

			m_data = MatrixAPI::FromRows(row0, row1, row2);
		}

		NU_FORCEINLINE Matrix3x3(const Vector3& row0, const Vector3& row1, const Vector3& row3) noexcept
		{

		}

		NU_FORCEINLINE Matrix3x3(const Matrix3x3& other) noexcept
			: m_data(other.m_data)
		{
		}

		NU_FORCEINLINE Matrix3x3(Matrix3x3&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		NU_FORCEINLINE Matrix3x3& operator=(const Matrix3x3& other) noexcept
		{
			if (this != &other)
			{
				m_data = other.m_data;
			}
			return *this;
		}

		NU_FORCEINLINE Matrix3x3& operator=(Matrix3x3&& other) noexcept
		{
			if (this != &other)
			{
				m_data = std::move(other.m_data);
			}
			return *this;
		}

		~Matrix3x3 = default;



		[[nodiscard]] NU_FORCEINLINE Matrix3x3 operator+(const Matrix3x3& other) noexcept
		{

		}

	private:
		MatrixAPI::NuMat3 m_data;

		explicit NU_FORCEINLINE Matrix3x3(const MatrixAPI::NuMat3& simdData) noexcept
			: m_data(simdData)
		{
		}
	};
}