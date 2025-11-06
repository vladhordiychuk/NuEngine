// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Algebra/Matrix/MatrixAPI.hpp>
#include <Math/Algebra/Vector/Vector4.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Quaternion/Quaternion.hpp>
#include <Core/Types/Types.hpp>
#include <Math/StorageTypes.hpp>

#include <string>
#include <sstream>
#include <iomanip>
#include <initializer_list>

namespace NuEngine::Math
{
	using NuInt32 = Core::Types::NuInt32;
	using NuFloat = Core::Types::NuFloat;
	using NuBool = Core::Types::NuBool;

	/**
	 * @brief A 4x4 matrix aligned to a 16-byte boundary for SIMD operations.
	 *
	 * Stores data in a format optimized for MatrixAPI (likely row-major or 
	 * column-major depending on the API implementation).
	 * Provides an interface for standard matrix operations.
	 */
	class alignas(16) Matrix4x4
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Initializes the matrix to an identity matrix.
		 */
		NU_FORCEINLINE Matrix4x4() noexcept
			: m_data(MatrixAPI::SetIdentityMatrix())
		{
		}

		/**
		 * @brief Constructs a matrix from 16 scalar values.
		 *
		 * Assumes the values are provided in row-major order.
		 * (m00, m01, m02, m03, ...)
		 *
		 * @param m00 Element at row 0, column 0.
		 * @param m01 Element at row 0, column 1.
		 * @param m02 Element at row 0, column 2.
		 * @param m03 Element at row 0, column 3.
		 * @param m10 Element at row 1, column 0.
		 * @param m11 Element at row 1, column 1.
		 * @param m12 Element at row 1, column 2.
		 * @param m13 Element at row 1, column 3.
		 * @param m20 Element at row 2, column 0.
		 * @param m21 Element at row 2, column 1.
		 * @param m22 Element at row 2, column 2.
		 * @param m23 Element at row 2, column 3.
		 * @param m30 Element at row 3, column 0.
		 * @param m31 Element at row 3, column 1.
		 * @param m32 Element at row 3, column 2.
		 * @param m33 Element at row 3, column 3.
		 */
		NU_FORCEINLINE Matrix4x4(NuFloat m00, NuFloat m01, NuFloat m02, NuFloat m03,
								 NuFloat m10, NuFloat m11, NuFloat m12, NuFloat m13,
								 NuFloat m20, NuFloat m21, NuFloat m22, NuFloat m23,
								 NuFloat m30, NuFloat m31, NuFloat m32, NuFloat m33) noexcept
		{
			Vector4 r0(m00, m01, m02, m03);
			Vector4 r1(m10, m11, m12, m13);
			Vector4 r2(m20, m21, m22, m23);
			Vector4 r3(m30, m31, m32, m33);

			m_data = MatrixAPI::FromRows(r0, r1, r2, r3);
		}

		/**
		 * @brief Constructs a matrix from four row vectors.
		 *
		 * @param row0 The first row (m00, m01, m02, m03).
		 * @param row1 The second row (m10, m11, m12, m13).
		 * @param row2 The third row (m20, m21, m22, m23).
		 * @param row3 The fourth row (m30, m31, m32, m33).
		 */
		NU_FORCEINLINE Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept
			: m_data(MatrixAPI::FromRows(row0, row1, row2, row3))
		{
		}

		/**
		 * @brief Constructs a matrix from an initializer list.
		 *
		 * Assumes the 16 values are provided in row-major order.
		 *
		 * @param list An initializer list containing 16 floats.
		 * @throws Assertion failure if list size is not 16.
		 */
		NU_FORCEINLINE Matrix4x4(std::initializer_list<NuFloat> list) noexcept
		{
			NuEngine::Core::Types::NuAssert(list.size() == 16, "Matrix4x4 initializer_list must contain exactly 16 elements!");
			auto it = list.begin();

			Vector4 r0(*it++, *it++, *it++, *it++);
			Vector4 r1(*it++, *it++, *it++, *it++);
			Vector4 r2(*it++, *it++, *it++, *it++);
			Vector4 r3(*it++, *it++, *it++, *it++);

			m_data = MatrixAPI::FromRows(r0, r1, r2, r3);
		}

		/**
		 * @brief Copy constructor.
		 *
		 * @param other The matrix to copy from.
		 */
		NU_FORCEINLINE Matrix4x4(const Matrix4x4& other) noexcept
			: m_data(other.m_data)
		{
		}

		/**
		 * @brief Move constructor.
		 *
		 * @param other The matrix to move from.
		 */
		NU_FORCEINLINE Matrix4x4(Matrix4x4&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}
		
		/**
         * @brief Copy assignment operator.
		 *
         * Assigns the values from another matrix to this instance.
         * Performs a check for self-assignment.
		 *
         * @param other The matrix to copy data from.
		 *
         * @return A reference to this matrix after assignment.
         */
		NU_FORCEINLINE Matrix4x4& operator=(const Matrix4x4& other) noexcept
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
		NU_FORCEINLINE Matrix4x4& operator=(Matrix4x4&& other) noexcept
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
		~Matrix4x4() = default;

		/**
		 * @brief Returns an identity matrix.
		 *
		 * @return An identity matrix.
		 */
		[[nodiscard]] static Matrix4x4 Identity() noexcept
		{
			return Matrix4x4(MatrixAPI::Identity());
		}

		/**
		 * @brief Creates a matrix from four row vectors.
		 *
		 * @param row0 The first row.
		 * @param row1 The second row.
		 * @param row2 The third row.
		 * @param row3 The fourth row.
		 *
		 * @return A new matrix.
		 */
		[[nodiscard]] static Matrix4x4 FromRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept
		{
			return Matrix4x4(MatrixAPI::FromRows(row0, row1, row2, row3));
		}

		/**
		 * @brief Creates a matrix from four column vectors.
		 *
		 * @param col0 The first column.
		 * @param col1 The second column.
		 * @param col2 The third column.
		 * @param col3 The fourth column.
		 *
		 * @return A new matrix.
		 */
		[[nodiscard]] static Matrix4x4 FromColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3) noexcept
		{
			return Matrix4x4(MatrixAPI::FromColumns(col0, col1, col2, col3));
		}

		/**
		 * @brief Multiplies this matrix by another matrix.
		 *
		 * @param other The right-hand side matrix.
		 *
		 * @return The result of the multiplication (this * other).
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 operator*(const Matrix4x4& other) const noexcept
		{
			return Matrix4x4(MatrixAPI::Mul(m_data, other.m_data));
		}

		/**
		 * @brief Multiplies this matrix by a 4D vector.
		 *
		 * @param other The vector to transform.
		 *
		 * @return The transformed vector.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 operator*(const Vector4& other) const noexcept
		{
			return Vector4(MatrixAPI::Mul(m_data, other.m_data));
		}

		/**
		 * @brief Performs component-wise addition of two matrices.
		 *
		 * @param other The matrix to add.
		 *
		 * @return The result of the addition.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 operator+(const Matrix4x4& other) const noexcept
		{
			return Matrix4x4(MatrixAPI::Add(m_data, other.m_data));
		}

		/**
		 * @brief Performs component-wise subtraction of two matrices.
		 *
		 * @param other The matrix to subtract.
		 *
		 * @return The result of the subtraction.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 operator-(const Matrix4x4& other) const noexcept
		{
			return Matrix4x4(MatrixAPI::Sub(m_data, other.m_data));
		}

		/**
		 * @brief Multiplies this matrix by another matrix and assigns the result.
		 *
		 * @param other The right-hand side matrix.
		 *
		 * @return A reference to this matrix.
		 */
		NU_FORCEINLINE Matrix4x4& operator*=(const Matrix4x4& other) noexcept
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
		NU_FORCEINLINE Matrix4x4& operator+=(const Matrix4x4& other) noexcept
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
		NU_FORCEINLINE Matrix4x4& operator-=(const Matrix4x4& other) noexcept
		{
			m_data = MatrixAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/**
         * @brief Equality comparison operator.
		 *
         * Checks if all components of this matrix are exactly equal
         * to the components of another matrix.
		 *
         * @param other The matrix to compare against.
		 *
         * @return true if all components are identical, false otherwise.
         */
		[[nodiscard]] NU_FORCEINLINE NuBool operator==(const Matrix4x4& other) const noexcept
		{
			return MatrixAPI::Equal(m_data, other.m_data);
		}

		/**
         * @brief Inequality comparison operator.
		 *
         * Checks if at least one component of this matrix differs
         * from the components of another matrix.
		 *
_        * @param other The matrix to compare against.
		 *
         * @return true if the matrices are not identical, false otherwise.
         */
		[[nodiscard]] NU_FORCEINLINE NuBool operator!=(const Matrix4x4& other) noexcept
		{
			return !(*this == other);
		}

		/**
		 * @brief Returns the transpose of this matrix.
		 *
		 * @return The transposed matrix.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 Transpose() const noexcept
		{
			return Matrix4x4(MatrixAPI::Transpose(m_data));
		}

		/**
		 * @brief Calculates the determinant of this matrix.
		 *
		 * @return The determinant value.
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat Determinant() const noexcept
		{
			return MatrixAPI::Determinant(m_data);
		}

		/**
		 * @brief Calculates the inverse of this matrix.
		 *
		 * @return The inverse matrix. Result is undefined if the determinant is zero.
		 */
		[[nodiscard]] NU_FORCEINLINE Matrix4x4 Inverse() const noexcept
		{
			return Matrix4x4(MatrixAPI::Inverse(m_data));
		}

		/**
		 * @brief Creates a translation matrix.
		 *
		 * @param translation The translation vector.
		 *
		 * @return A translation matrix.
		 */
		[[nodiscard]] static NU_FORCEINLINE Matrix4x4 CreateTranslation(const Vector3& translation) noexcept
		{
			// Fixed: Static method doesn't have m_data
			return Matrix4x4(MatrixAPI::CreateTranslation(translation));
		}

		/**
		 * @brief Creates a rotation matrix from a quaternion.
		 *
		 * @param rotation The rotation quaternion.
		 *
		 * @return A rotation matrix.
		 */
		[[nodiscard]] static NU_FORCEINLINE Matrix4x4 CreateRotation(const Quaternion& rotation) noexcept
		{
			return Matrix4x4(MatrixAPI::CreateRotation(rotation));
		}

		/**
		 * @brief Creates a scale matrix.
		 *
		 * @param scale The scaling vector (X, Y, Z).
		 *
		 * @return A scale matrix.
		 */
		[[nodiscard]] static NU_FORCEINLINE Matrix4x4 CreateScale(const Vector3& scale) noexcept
		{
			return Matrix4x4(MatrixAPI::CreateScale(scale));
		}

		/**
		 * @brief Creates a look-at view matrix.
		 *
		 * @param eye The position of the camera.
		 * @param target The point the camera is looking at.
		 * @param up The up vector of the camera (usually {0, 1, 0}).
		 *
		 * @return A view matrix.
		 */
		[[nodiscard]] static NU_FORCEINLINE Matrix4x4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept
		{
			return Matrix4x4(MatrixAPI::CreateLookAt(eye, target, up));
		}

		/**
		 * @brief Creates a perspective projection matrix.
		 *
		 * @param fovY Field of view in the Y direction (in radians).
		 * @param aspect Aspect ratio (width / height).
		 * @param nearZ Distance to the near clipping plane.
		 * @param farZ Distance to the far clipping plane.
		 *
		 * @return A perspective projection matrix.
		 */
		[[nodiscard]] static NU_FORCEINLINE Matrix4x4 CreatePerspective(NuFloat fovY, NuFloat aspect, NuFloat nearZ, NuFloat farZ) noexcept
		{
			return Matrix4x4(MatrixAPI::CreatePerspective(fovY, aspect, nearZ, farZ));
		}

		/**
		 * @brief Creates an orthographic projection matrix.
		 * 
		 * @param left The left bound of the view volume.
		 * @param right The right bound of the view volume.
		 * @param bottom The bottom bound of the view volume.
		 * @param top The top bound of the view volume.
		 * @param nearZ Distance to the near clipping plane.
		 * @param farZ Distance to the far clipping plane.
		 * 
		 * @return An orthographic projection matrix.
		 */
		[[nodiscard]] static NU_FORCEINLINE Matrix4x4 CreateOrthographic(NuFloat left, NuFloat right, NuFloat bottom, NuFloat top, NuFloat nearZ, NuFloat farZ) noexcept
		{
			return Matrix4x4(MatrixAPI::CreateOrthographic(left, right, bottom, top, nearZ, farZ));
		}

		/**
		 * @brief Decomposes the transformation matrix into its components.
		 *
		 * @param[out] translation The translation vector.
		 * @param[out] rotation The rotation quaternion.
		 * @param[out] scale The scale vector.
		 */
		NU_FORCEINLINE void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const noexcept
		{
			MatrixAPI::Decompose(m_data, translation, rotation, scale);
		}

		/**
		 * @brief Gets a specified column of the matrix as a Vector4.
		 *
		 * @param index The column index (0-3).
		 *
		 * @return A Vector4 representing the column.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 GetColumn(NuInt32 index) const noexcept
		{
			return Vector4(MatrixAPI::GetColumn(m_data, index));
		}

		/**
		 * @brief Gets a specified row of the matrix as a Vector4.
		 *
		 * @param index The row index (0-3).
		 *
		 * @return A Vector4 representing the row.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 GetRow(NuInt32 index) const noexcept
		{
			return Vector4(MatrixAPI::GetRow(m_data, index));
		}

		/**
		 * @brief Returns a pointer to the matrix's data (16 floats).
		 *
		 * @return A const pointer to the first element.
		 */
		[[nodiscard]] NU_FORCEINLINE const NuFloat* Data() const noexcept
		{
			// Assumes NuMat4 can be safely cast to NuFloat*
			return reinterpret_cast<const NuFloat*>(&m_data);
		}

		/**
		 * @brief Accesses an element of the matrix using row and column indices.
		 *
		 * @param row The row index (0-3).
		 * @param col The column index (0-3).
		 *
		 * @return The value of the element.
		 */
		[[nodiscard]] NU_FORCEINLINE NuFloat operator()(NuInt32 row, NuInt32 col) const noexcept
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
		[[nodiscard]] NU_FORCEINLINE NuBool IsIdentity(NuFloat epsilon = 1e-6f) const noexcept
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
			
			for (NuInt32 row = 0; row < 4; ++row)
			{
				oss << "[";
				for (NuInt32 col = 0; col < 4; ++col)
				{
					oss << (*this)(row, col);
					if (col < 3)
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
		friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& mat)
		{
			os << mat.ToString();
			return os;
		}

	private:
		/**
		 * @brief Internal data storage.
		 */
		MatrixAPI::NuMat4 m_data;

		/**
		 * @brief Private constructor to create a matrix from the internal API type.
		 *
		 * @param simdData Data in MatrixAPI::NuMat4 format.
		 */
		explicit NU_FORCEINLINE Matrix4x4(const MatrixAPI::NuMat4& simdData) noexcept
			: m_data(simdData)
		{
		}
	};
}