// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Algebra/Matrix/MatrixAPI.hpp>
#include <NuMath/Algebra/Vector/Vector2.hpp>
#include <NuMath/Core/Common.hpp>

#include <initializer_list>
#include <string>
#include <sstream>
#include <iomanip>

namespace NuMath
{
	/**
	 * @brief A 2x2 matrix aligned to 16-byte boundary for SIMD operations.
	 * * Stores data in a format optimized for MatrixAPI.
	 * Provides an interface for standard matrix operations.
	 */
	class alignas(16) Matrix2x2
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Initializes the matrix to an identity matrix.
		 */
		NU_FORCEINLINE Matrix2x2() noexcept
			: m_data(MatrixAPI::SetIdentityMatrix2x2())
		{
		}

		/**
		 * @brief Constructs a matrix from 4 scalar values.
		 *
		 * Assumes the values are provided in row-major order.
		 * (m00, m01)
		 * (m10, m11)
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
		 * * @param row0 The first row (m00, m01).
		 * @param row1 The second row (m10, m11).
		 */
		NU_FORCEINLINE Matrix2x2(const Vector2& row0, const Vector2& row1) noexcept
			: m_data(MatrixAPI::FromRows(row0.SimdData(), row1.SimdData()))
		{
		}

		/**
		 * @brief Constructs a matrix from an initializer list.
		 *
		 * Assumes the 4 values are provided in row-major order.
		 *
		 * @param list An initializer list containing 4 floats.
		 */
		NU_FORCEINLINE Matrix2x2(std::initializer_list<float> list) noexcept
		{
			NU_MATH_ASSERT(list.size() == 4, "Matrix2x2 initializer_list must contain exactly 4 elements!");

			auto it = list.begin();
			Vector2 r0(it[0], it[1]);
			Vector2 r1(it[2], it[3]);

			m_data = MatrixAPI::FromRows(r0.SimdData(), r1.SimdData());
		}

		/**
		 * @brief Copy constructor.
		 */
		NU_FORCEINLINE Matrix2x2(const Matrix2x2& other) noexcept = default;

		/**
		 * @brief Move constructor.
		 */
		NU_FORCEINLINE Matrix2x2(Matrix2x2&& other) noexcept = default;

		/**
		 * @brief Private constructor to create a matrix from the internal API type.
		 */
		NU_FORCEINLINE Matrix2x2(const MatrixAPI::NuMat2& simdData) noexcept
			: m_data(simdData)
		{
		}

		/**
		 * @brief Copy assignment operator.
		 */
		NU_FORCEINLINE Matrix2x2& operator=(const Matrix2x2& other) noexcept = default;

		/**
		 * @brief Move assignment operator.
		 */
		NU_FORCEINLINE Matrix2x2& operator=(Matrix2x2&& other) noexcept = default;

		/**
		 * @brief Destructor.
		 */
		~Matrix2x2() = default;

		/**
		 * @brief Returns an identity matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix2x2 Identity() noexcept
		{
			return Matrix2x2(MatrixAPI::SetIdentityMatrix2x2());
		}

		/**
		 * @brief Creates a matrix from two row vectors.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix2x2 FromRows(const Vector2& row0, const Vector2& row1) noexcept
		{
			return Matrix2x2(MatrixAPI::FromRows(row0.SimdData(), row1.SimdData()));
		}

		/**
		 * @brief Creates a matrix from two column vectors.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix2x2 FromColumns(const Vector2& col0, const Vector2& col1) noexcept
		{
			return Matrix2x2(MatrixAPI::FromColumns(col0.SimdData(), col1.SimdData()));
		}

		/**
		 * @brief Multiplies this matrix by another matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix2x2 operator*(const Matrix2x2& other) const noexcept
		{
			return Matrix2x2(MatrixAPI::Mul(m_data, other.m_data));
		}

		/**
		 * @brief Multiplies this matrix by a 2D vector.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator*(const Vector2& other) const noexcept
		{
			return Vector2(MatrixAPI::Mul(m_data, other.SimdData()));
		}

		/**
		 * @brief Performs component-wise addition of two matrices.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix2x2 operator+(const Matrix2x2& other) const noexcept
		{
			return Matrix2x2(MatrixAPI::Add(m_data, other.m_data));
		}

		/**
		 * @brief Performs component-wise subtraction of two matrices.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix2x2 operator-(const Matrix2x2& other) const noexcept
		{
			return Matrix2x2(MatrixAPI::Sub(m_data, other.m_data));
		}

		/**
		 * @brief Multiplies this matrix by another matrix and assigns the result.
		 */
		NU_FORCEINLINE Matrix2x2& operator*=(const Matrix2x2& other) noexcept
		{
			m_data = MatrixAPI::Mul(m_data, other.m_data);
			return *this;
		}

		/**
		 * @brief Performs component-wise addition and assigns the result.
		 */
		NU_FORCEINLINE Matrix2x2& operator+=(const Matrix2x2& other) noexcept
		{
			m_data = MatrixAPI::Add(m_data, other.m_data);
			return *this;
		}

		/**
		 * @brief Performs component-wise subtraction and assigns the result.
		 */
		NU_FORCEINLINE Matrix2x2& operator-=(const Matrix2x2& other) noexcept
		{
			m_data = MatrixAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/**
		 * @brief Equality comparison operator.
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator==(const Matrix2x2& other) const noexcept
		{
			return MatrixAPI::Equal(m_data, other.m_data);
		}

		/**
		 * @brief Inequality comparison operator.
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator!=(const Matrix2x2& other) const noexcept
		{
			return !(*this == other);
		}

		/**
		 * @brief Returns the transpose of this matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix2x2 Transpose() const noexcept
		{
			return Matrix2x2(MatrixAPI::Transpose(m_data));
		}

		/**
		 * @brief Calculates the determinant of this matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE float Determinant() const noexcept
		{
			return MatrixAPI::Determinant(m_data);
		}

		/**
		 * @brief Calculates the inverse of this matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix2x2 Inverse() const noexcept
		{
			return Matrix2x2(MatrixAPI::Inverse(m_data));
		}

		/**
		 * @brief Gets a specified column of the matrix as a Vector2.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 GetColumn(int index) const noexcept
		{
			return Vector2(MatrixAPI::GetColumn(m_data, index));
		}

		/**
		 * @brief Gets a specified row of the matrix as a Vector2.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 GetRow(int index) const noexcept
		{
			return Vector2(MatrixAPI::GetRow(m_data, index));
		}

		/**
		 * @brief Sets the values of a specific column.
		 */
		NU_FORCEINLINE void SetColumn(int index, const Vector2& column) noexcept
		{
			MatrixAPI::SetColumn(m_data, index, column.SimdData());
		}

		/**
		 * @brief Sets the values of a specific row.
		 */
		NU_FORCEINLINE void SetRow(int index, const Vector2& row) noexcept
		{
			MatrixAPI::SetRow(m_data, index, row.SimdData());
		}

		/**
		 * @brief Returns a pointer to the matrix's data.
		 */
		[[nodiscard]] NU_FORCEINLINE const float* Data() const noexcept
		{
			return reinterpret_cast<const float*>(&m_data);
		}

		/**
		 * @brief Returns a pointer to the matrix's data.
		 */
		[[nodiscard]] NU_FORCEINLINE float* Data() noexcept
		{
			return reinterpret_cast<float*>(&m_data);
		}

		/**
		 * @brief Accesses an element of the matrix using row and column indices.
		 */
		[[nodiscard]] NU_FORCEINLINE float operator()(int row, int col) const noexcept
		{
			return MatrixAPI::Access(m_data, row, col);
		}

		/**
		 * @brief Accesses an element of the matrix using row and column indices for read/write.
		 */
		[[nodiscard]] NU_FORCEINLINE float& operator()(int row, int col) noexcept
		{
			return MatrixAPI::Access(m_data, row, col);
		}

		/**
		 * @brief Checks if the matrix is an identity matrix (within an epsilon).
		 */
		[[nodiscard]] NU_FORCEINLINE bool IsIdentity(float epsilon = NuMath::EPSILON) const noexcept
		{
			return MatrixAPI::IsIdentity(m_data, epsilon);
		}

		/**
		 * @brief Returns a string representation of the matrix.
		 */
		[[nodiscard]] std::string ToString() const
		{
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(5);

			for (int row = 0; row < 2; ++row)
			{
				oss << "[";
				for (int col = 0; col < 2; ++col)
				{
					oss << (*this)(row, col);
					if (col < 1)
					{
						oss << ",\t";
					}
				}
				oss << "]\n";
			}
			return oss.str();
		}

		/**
		 * @brief Output stream operator.
		 */
		friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& mat)
		{
			os << mat.ToString();
			return os;
		}

	private:
		/**
		 * @brief Internal data storage.
		 */
		MatrixAPI::NuMat2 m_data;
	};
} // namespace NuMath