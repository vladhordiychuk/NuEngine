// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Algebra/Matrix/MatrixAPI.hpp>
#include <NuMath/Algebra/Vector/Vector3.hpp>
#include <NuMath/Core/Common.hpp>

#include <string>
#include <sstream>
#include <iomanip>
#include <initializer_list>

namespace NuMath
{
	/**
	 * @brief A 3x3 matrix aligned to 16-byte boundary for SIMD.
	 * 
	 * Stores data in a format optimized for MatrixAPI (likely row-major or
	 * column-major depending on the API implementation).
	 * Provides an interface for standard matrix operations.
	 */
	class alignas(16) Matrix3x3
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Initializes the matrix to an identity matrix.
		 */
		NU_FORCEINLINE Matrix3x3() noexcept
			: m_data(MatrixAPI::SetIdentityMatrix())
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
		NU_FORCEINLINE Matrix3x3(float m00, float m01, float m02,
								 float m10, float m11, float m12,
								 float m20, float m21, float m22) noexcept
		{
			Vector3 row0(m00, m01, m02);
			Vector3 row1(m10, m11, m12);
			Vector3 row2(m20, m21, m22);

			m_data = MatrixAPI::FromRows(row0.SimdData(), row1.SimdData(), row2.SimdData());
		}

		/**
		 * @brief Constructs a matrix from three row vectors.
		 * 
		 * @param row0 The first row (m00, m01, m02).
		 * @param row1 The second row (m10, m11, m12).
		 * @param row2 The third row (m20, m21, m22).
		 */
		NU_FORCEINLINE Matrix3x3(const Vector3& row0, const Vector3& row1, const Vector3& row2) noexcept
			: m_data(MatrixAPI::FromRows(row0.SimdData(), row1.SimdData(), row2.SimdData()))
		{
		}

		/**
		 * @brief Constructs a matrix from three column vectors.
		 *
		 * @param col0 The first column (m00, m01, m02).
		 * @param col1 The second column (m10, m11, m12).
		 * @param col2 The third column (m20, m21, m22).
		 */
		NU_FORCEINLINE Matrix3x3(const Vector3& col0, const Vector3& col1, const Vector3& col2) noexcept
			: m_data(MatrixAPI::FromColumns(col0.SimdData(), col1.SimdData(), col2.SimdData()))
		{
		}

		/**
		 * @brief Constructs a matrix from an initializer list.
		 * 
		 * Assumes the 9 values are provided in row-major order.
		 * 
		 * @param list An initializer list containing 9 floats.
		 * @throws Assertion failure if list size is not 9.
		 */
		NU_FORCEINLINE Matrix3x3(std::initializer_list<float> list) noexcept
		{
			NU_MATH_ASSERT(list.size() == 9, "Matrix3x3 initializer_list must contain exactly 9 elements!");
			auto it = list.begin();

			Vector3 r0(*it++, *it++, *it++);
			Vector3 r1(*it++, *it++, *it++);
			Vector3 r2(*it++, *it++, *it++);

			m_data = MatrixAPI::FromRows(r0.SimdData(), r1.SimdData(), r2.SimdData());
		}

		/**
		 * @brief Copy constructor.
		 * 
		 * @param other The matrix to copy from.
		 */
		NU_FORCEINLINE Matrix3x3(const Matrix3x3& other) noexcept
			: m_data(other.m_data)
		{
		}

		/**
		 * @brief Move constructor.
		 * 
		 * @param other The matrix to move from.
		 */
		NU_FORCEINLINE Matrix3x3(Matrix3x3&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		/**
		 * @brief Copy assignment operator.
		 * 
		 * Assigns the value from another matrix to this instance.
		 * Performs a check for self-assignment.
		 * 
		 * @param other The matrix to copy data from.
		 * 
		 * @return A reference to this matrix after assignment.
		 */
		NU_FORCEINLINE Matrix3x3& operator=(const Matrix3x3& other) noexcept
		{
			if (this != &other)
			{
				m_data = other.m_data;
			}
			return *this;
		}

		/**
		 * @brief Move assignment operator.
		 * 
		 * Moves the data from another matrix into this instance,
		 * leaving other in a valid but unspecified state.
		 * 
		 * @param other The r-value matrix to move data from.
		 * 
		 * @return A reference to this matrix after the move.
		 */
		NU_FORCEINLINE Matrix3x3& operator=(Matrix3x3&& other) noexcept
		{
			if (this != &other)
			{
				m_data = std::move(other.m_data);
			}
			return *this;
		}

		/**
		 * @brief Destructor.
		 */
		~Matrix3x3() = default;

		/**
		 * @brief Returns an identity matrix.
		 * 
		 * @return An identity matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix3x3 Identity() noexcept
		{
			return Matrix3x3(MatrixAPI::SetIdentityMatrix3x3());
		}

		/**
		 * @brief Creates a matrix from three row vector.
		 * 
		 * @param row0 The first row.
		 * @param row1 The second row.
		 * @param row2 The third row.
		 * 
		 * @return A new matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix3x3 FromRows(const Vector3& row0, const Vector3& row1, const Vector3& row2) noexcept
		{
			return Matrix3x3(MatrixAPI::FromRows(row0, row1, row2));
		}

		/**
		 * @brief Creates a matrix from three column vector.
		 * 
		 * @param col0 The first column.
		 * @param col1 The second column.
		 * @param col2 The third column.
		 * 
		 * @return A new matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix3x3 FromColumns(const Vector3& col0, const Vector3& col1, const Vector3& col2) noexcept
		{
			return Matrix3x3(MatrixAPI::FromColumns(col0, col1, col2));
		}

		/**
		 * @brief Multiplies this matrix by another matrix.
		 * 
		 * @param other The right-hand side matrix.
		 * 
		 * @return The result of the multiplication (this * other).
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix3x3 operator*(const Matrix3x3& other) noexcept
		{
			return Matrix3x3(MatrixAPI::Mul(m_data, other.m_data));
		}

		/** 
		 * @brief Multiplies this matrix by a 3D vector.
		 * 
		 * @param other The vector to transform.
		 * 
		 * @return The transformed vector.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix3x3 operator*(const Vector3& vec) noexcept
		{
			return Vector3(MatrixAPI::Mul(m_data, vec.SimdData()));
		}

		/** 
		 * @brief Performs component-wise addition of two matrices.
		 * 
		 * @param other The matrix to add.
		 * 
		 * @return The result of the addition.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix3x3 operator+(const Matrix3x3& other) noexcept
		{
			return Matrix3x3(MatrixAPI::Add(m_data, other.m_data));
		}

		/** 
		 * @brief Performs component-wise subtraction of two matrices.
		 * 
		 * @param other The matrix to subtract.
		 * 
		 * @return The result of subtraction.
		 */		
		[[nodiscard]] NU_FORCEINLINE Matrix3x3 operator-(const Matrix3x3& other) noexcept
		{
			return Matrix3x3(MatrixAPI::Sub(m_data, other.m_data));
		}

		/**
		 * @brief Multiplies this matrix by another matrix and assigns the result.
		 * 
		 * @param other The right-hand side matrix.
		 * 
		 * @return A reference to this matrix.
		 */
		NU_FORCEINLINE Matrix3x3& operator*=(const Matrix3x3& other) noexcept
		{
			m_data = MatrixAPI::Mul(m_data, other.m_data);
			return *this;
		}

		/** 
		 * @brief Performs component-wise addition and assigns the result.
		 * 
		 * @param other The matrix to add.
		 * 
		 * @return A reference to this matrix.
		 */		
		NU_FORCEINLINE Matrix3x3& operator+=(const Matrix3x3& other) noexcept
		{
			m_data = MatrixAPI::Add(m_data, other.m_data);
			return *this;
		}

		/** 
		 * @brief Performs component-wise subtraction and assigns the result.
		 * 
		 * @param other The matrix to subtract.
		 * 
		 * @return A reference to this matrix.
		 */
		NU_FORCEINLINE Matrix3x3& operator-=(const Matrix3x3& other) noexcept
		{
			m_data = MatrixAPI::Sub(m_data, other.m_data);
			return *this;
		}


		[[nodiscard]] NU_FORCEINLINE bool operator==(const Matrix3x3& other) const noexcept
		{
			return MatrixAPI::Equal(m_data, other.m_data);
		}

		[[nodiscard]] NU_FORCEINLINE bool operator!=(const Matrix3x3& other) const noexcept
		{
			return !(*this == other);
		}

		/**
		 * @brief Returns the transpose of the matrix.
		 * 
		 * @return The transposed matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix3x3 Transpose() const noexcept
		{
			return Matrix3x3(MatrixAPI::Transpose(m_data));
		}

		/**
		 * @brief Calculates the determinant of this matrix.
		 *
		 * @return The determinant value.
		 */
		[[nodiscard]] NU_FORCEINLINE float Determinant() const noexcept
		{
			return MatrixAPI::Determinant(m_data);
		}

		/**
		 * @brief Calculates the inverse of this matrix.
		 *
		 * @return The inverse matrix. Result is undefined if the determinant is zero.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix3x3 Inverse() const noexcept
		{
			return Matrix3x3(MatrixAPI::Inverse(m_data));
		}

		/**
		 * @brief Creates a rotation matrix from a quaternion.
		 *
		 * @param rotation The rotation quaternion.
		 *
		 * @return A rotation matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix3x3 CreateRotation(const Vector3& rotation) noexcept
		{
			return Matrix3x3(MatrixAPI::CreateRotation(rotation.SimdData()));
		}

		/**
		 * @brief Creates a scale matrix.
		 *
		 * @param scale The scaling vector (X, Y, Z).
		 *
		 * @return A scale matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE static Matrix3x3 CreateScale(const Vector3& scale) noexcept
		{
			return Matrix3x3(MatrixAPI::CreateScale(scale.SimdData()));
		}

		/**
		 * @brief Gets a specified column of the matrix as a Vector3.
		 * 
		 * @param index The column index (0-2).
		 *
		 * @return A Vector3 representing the column.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector3 GetColumn(int index) const noexcept
		{
			return Vector3(MatrixAPI::GetColumn(m_data, index));
		}

		/**
		 * @brief Gets a specified row of the matrix as a Vector3.
		 * 
		 * @param index The row index (0-2).
		 *
		 * @return A Vector3 representing the row.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector3 GetRow(int index) const noexcept
		{
			return Vector3(MatrixAPI::GetRow(m_data, index));
		}

		/**
		 * @brief Sets the value of a specified column.
		 * 
		 * Efficient way to update a column using SIMD registers
		 * instead of setting individual scalar components.
		 * 
		 * @param index The index of the column to set (0-2).
		 * @param column The vector containing the new values for the column.
		 */
		NU_FORCEINLINE void SetColumn(int index, const Vector3& column) noexcept
		{
			m_data = MatrixAPI::SetColumn(m_data, index, column.SimdData());
		}

		/**
		 * @brief Sets the value of a specified row.
		 *
		 * Efficient way to update a row using SIMD registers
		 * instead of setting individual scalar components.
		 * 
		 * @param index The index of the row to set (0-2).
		 * @param row The vector containing the new values for the row.
		 */
		NU_FORCEINLINE void SetRow(int index, const Vector3& row) noexcept
		{
			m_data = MatrixAPI::SetRow(m_data, index, row.SimdData());
		}
		
		/**
		 * @brief Returns a pointer to the matrix's data (9 floats).
		 * 
		 * @return A const pointer to the first element.
		 *
		 */
		[[nodiscard]] NU_FORCEINLINE const float* Data() const noexcept
		{
			return reinterpret_cast<const float*>(&m_data);
		}

		/**
		 * @brief Returns a pointer to the matrix's data (9 floats).
		 * 
		 * @return A pointer to the first element.
		 */
		[[nodiscard]] NU_FORCEINLINE float* Data() noexcept
		{
			return reinterpret_cast<float*>(&m_data);
		}

		/**
		 * @brief Accesses an element ot the matrix using row and column indices.
		 *
		 * @param row The row index (0-2).
		 * @param col The column index (0-2).
		 * 
		 * @return The value of the element.
		 */
		[[nodiscard]] NU_FORCEINLINE float operator()(int row, int col) const noexcept
		{
			return MatrixAPI::Access(m_data, row, col);
		}

		/**
		 * @brief Accesses an element of the matrix using row and column indices for read/write.
		 * 
		 * @param row The row index (0-2).
		 * @param col The column index (0-2).
		 *
		 * @return A reference to the element at the specified location.
		 */
		NU_FORCEINLINE float& operator()(int row, int col) noexcept
		{
			return MatrixAPI::Access(m_data, row, col);
		}

		/**
		 * @brief Checks if the matrix is an identity matrix (within an epsilon).
		 * 
		 * @param epsilon The tolerance for floating-point comparison.
		 * 
		 * @return true if the matrix is identity, false otherwise.
		 */
		[[nodiscard]] NU_FORCEINLINE bool IsIdentity(float epsilon = 1e-6f) const noexcept
		{
			return MatrixAPI::IsIdentity(m_data, epsilon);
		}

		/** 
		 * @brief Returns a string representation of the matrix.
		 * 
		 * @return A std::string.
		 */
		[[nodiscard]] std::string ToString() const
		{
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(5);

			oss << "[";
			for (int row = 0; row < 3; ++row)
			{
				for (int col = 0; col < 3; ++col)
				{
					oss << (*this)(row, col);
					if (col < 2)
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
		 * 
		 * @param os The output stream.
		 * @param mat The matrix to output.
		 * 
		 * @return A reference to the output stream.
		 */
		friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& mat)
		{
			os << mat.ToString();
			return os;
		}

	private:
		/**
		 * @brief Internal data storage.
		 */
		MatrixAPI::NuMat3 m_data;

		/**
		 * @brief Private constructor ro create a matrix from the internal API type.
		 * 
		 * @param simdData Data in MatrixAPI::NuMat4 format.
		 */
		explicit NU_FORCEINLINE Matrix3x3(const MatrixAPI::NuMat3& simdData) noexcept
			: m_data(simdData)
		{
		}
	};
} // namespace NuMath