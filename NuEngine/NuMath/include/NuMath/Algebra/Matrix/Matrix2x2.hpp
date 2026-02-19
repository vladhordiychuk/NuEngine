// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#include <NuMath/Algebra/Matrix/MatrixAPI.hpp>
#include <NuMath/Algebra/Vector/Vector2.hpp>
#include <NuMath/Core/Common.hpp>

#include <initializer_list>
#include <string>
#include <sstream>
#include <iomanip>
#include <ini>

namespace NuMath
{
	/**
	 * @brief A 2x2 matrix aligned to 16-byte boundary for SIMD.
	 * 
	 * Stores data in a format optimized for MatrixAPI (likely row-major or
	 * column-major depending on the API implementation).
	 * Provides an interface for standart matrix operations.
	 */
	class alignas(16) Matrix2x2
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Initializes the matrix to an identity matrix.
		 */
		NU_FORCEINLU Matrix2x2() noexcept
			: m_data(MatrixAPI::SetIdentityMatrix2x2()
		{
		}

		/**
		 * @brief Constructs a matrix from 4 scalar values.
		 *
		 * Assumes the values are provided in row-major order.
		 * (m00, m01, m02, m03)
		 *
		 * @param m00 Element at row 0, column 0.
		 * @param m01 Element at row 0, column 1.
		 * @param m10 Element at row 1, column 0.
		 * @param m11 Element at row 1, column 1.
		 */
		NU_FORCEINLINE Matrix2x2(float m00, float m01,
								 float m10, float m11) noexcept
		{
			Vector2 row0(m00, m01);
			Vector2 row1(m10, m11);

			m_data = MatrixAPI::FromRows(row0.SimdData(), row1.SimdData());
		}

		/**
		 * @brief Constructs a matrix from two row vectors.
		 * 
		 * @param row0 The first row (m00, m01).
		 * @param row1 The second row (m10, m11).
		 */
		NU_FORCEINLINE Matrix2x2(const Vector2& row0, const Vector3& row1) noexcept
			: m_data(MatrixAPI::FromRows(row0.SimdData(), row1.SimdData())
		{
		}


		NU_FORCEINLINE Matrix2x2(const Vector2& col0, const Vector2& col1) noexcept
			: m_data(MatrixAPI::FromColumns(col0.SimdData(), col1.SimdData())
		{

		}

		NU_FORCEINLINE Matrix2x2(std::initializer_list<float> list) noexcept
		{
			NU_MATH_ASSERT(list.size() == 4, "Matrix2x2 inilializer_list must contain exactly 4 elements!");

		}

		NU_FORCEINLINE Matrix2x2(const Matrix2x2& other) noexcept
			: m_data(other.m_data)
		{
		}

		NU_FORCEINLINE Matrix2x2(Matrix2x2&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

	private:
		MatrixAPI::NuMat2 m_data;
	};
} // namespace NuMath