// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#define NU_FORCEINLINE __forceinline
#else
#define NU_FORCEINLINE inline __attribute__((always_inline))
#endif

namespace NuEngine::Math::Simd_Scalar
{
	// =============================================
	// Common types
	// =============================================

	/*
	* @brief 4-component vector type with 16-byte alignment.
	* 
	* Represents a SIMD-friendly vector with four float components (x, y, z, w).
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuVec4
	{
		float x, y, z, w;
	};

	/*
	* @brief 4x4 matrix type with 16-byte alignment.
	* 
	* Represents a column-major 4x4 matrix stored in a 2D array.
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) SimdMat4
	{
		float mat[4][4];
	};

	// =============================================
	// Vectors
	// =============================================

	// =============================================
	// Setters
	// =============================================

	/*
	* @brief Create a 4-component vector with specified values.
	* 
	* Initializes a NuVec4 with the given component values.
	* The w component is optional and defaults to 0.0f.
	* 
	* @param x The x-component value.
	* @param y The y-component value.
	* @param z The z-component value.
	* @param w The w-component value.
	* 
	* @return New NuVec4 with the specified component values.
	*/
	NU_FORCEINLINE NuVec4 Set(float x, float y, float z, float w = 0.0f)
	{
		return { x, y, z, w };
	}

	/*
	* @brief Create a 4-component vector with identical values.
	*
	* Initializes all components (x, y, z, w) to the same scalar value.
	*
	* @param scalar Value to set for all components.
	* 
	* @return New NuVec4 with all components equal to scalar.
	*/
	NU_FORCEINLINE NuVec4 SetAll(float scalar)
	{
		return { scalar, scalar, scalar, scalar };
	}

	/*
	* @brief Create a 4-component vector with zero values.
	* 
	* Initializes all components (x, y, z, w) to the zero.
	* 
	* @return New NuVec4 with all components equal to zero.
	*/
	NU_FORCEINLINE NuVec4 SetZero()
	{
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// =============================================
	// Getters
	// =============================================

	/*
	* @brief Get the X component of 4D vector.
	*
	* Extracts and returns the first component (X) of the vector.
	*
	* @param v Input vector.
	* 
	* @return float Value of X component.
	*/
	NU_FORCEINLINE float GetX(const NuVec4& v)
	{
		return v.x;
	}

	/*
	* @brief Get the Y component of 4D vector.
	* 
	* Extracts and returns the second component (Y) of the vector.
	* 
	* @param v Input vector.
	* 
	* @return float Value of Y component.
	*/
	NU_FORCEINLINE float GetY(const NuVec4& v)
	{
		return v.y;
	}

	/*
	* @brief Get the Z component of 4D vector.
	* 
	* Extracts and returns the third component (Z) of the vector.
	* 
	* @param v Input vector.
	* 
	* @return float Value of Z component.
	*/
	NU_FORCEINLINE float GetZ(const  NuVec4& v)
	{
		return v.z;
	}

	/*
	* @brief Get the W component of 4D vector.
	* 
	* Extracts and returns the fouth component(W) of the vector.
	* 
	* @param v Input vector.
	* 
	* @return float Value of W component.
	*/
	NU_FORCEINLINE float GetW(const NuVec4& v)
	{
		return v.w;
	}

	// =============================================
	// Basic Arithmetic
	// =============================================

	/*
	* @brief Component-wise addition of two 4D vectors.
	*
	* Performs scalar addition.
	*
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return New NuVec4 where each component is a[i] + b[i].
	*/
	NU_FORCEINLINE NuVec4 Add(const NuVec4& a, const NuVec4& b)
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	}

	/*
	* @brief Component-wise subtaction of two 4D vectors.
	* 
	* Perfoem scalar subtraction.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return New NuVec4 where each component is a[i] - b[i].
	*/
	NU_FORCEINLINE NuVec4 Sub(const NuVec4& a, const NuVec4& b)
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	}

	/*
	* @brief Component-wise multiplication of two 4D vectors.
	* 
	* Perform scalar multiplication.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return New NuVec4 where each component a[i] * b[i].
	*/
	NU_FORCEINLINE NuVec4 Mul(const NuVec4& a, const NuVec4& b)
	{
		return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
	}

	/*
	* @brief Component-wise division of two 4D vectors.
	* 
	* Perform scalar division.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return New NuVec4 where each component a[i] / b[i].
	*/
	NU_FORCEINLINE NuVec4 Div(const NuVec4& a, const NuVec4& b)
	{
		return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
	}

	/*
	* @brief Negates all components of a 4D vector.
	* 
	* Computes component-wise negation: [-x, -y, -z, -w].
	* 
	* @param a Input vector.
	* 
	* @return New NuVec4 with negated components.
	*/
	NU_FORCEINLINE NuVec4 Neg(const NuVec4& a)
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	/*
	* @brief Component-wise minimum of two vectors.
	* 
	* Computes a new vector where each component is the minimum
	* of corresponding components from input vectors.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return New NuVec4 where each component is min(a[i], b[i]).
	*/
	NU_FORCEINLINE NuVec4 Min(const NuVec4& a, const NuVec4& b)
	{
		return {
			(a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y,
			(a.z < b.z) ? a.z : b.z,
			(a.w < b.w) ? a.w : b.w
		};
	}

	/*
	* @brief Component-wise maximum of two vectors.
	* 
	* Computes a new vector where each component is the maximum
	* of corresponding components from input vectors.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return New NuVec4 where each component is max(a[i], b[i]).
	*/
	NU_FORCEINLINE NuVec4 Max(const NuVec4& a, const NuVec4& b)
	{
		return {
			(a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y,
			(a.z > b.z) ? a.z : b.z,
			(a.w > b.w) ? a.w : b.w
		};
	}

	/*
	* @brief Component-wise absolute value.
	* 
	* Returns vector with |x|, |y|, |z|, |w| components
	* 
	* @param v Input vector.
	* 
	* @return New NuVec4 with absolute values.
	*/
	NU_FORCEINLINE NuVec4 Abs(const NuVec4& v)
	{
		return { std::fabs(v.x), std::fabs(v.y), std::fabs(v.z), std::fabs(v.w) };
	}

	/*
	* @brief Exact floating-point vector equality comparison.
	* 
	* Performs component-wise == comparison. For approximate equality,
	* consider using a threshold-based comparison instead.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return true if all components are exactly equal.
	* @warning Floating-point imprecision may affect results.
	*/
	NU_FORCEINLINE bool Equal(const NuVec4& a, const NuVec4& b)
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
	}

	/*
	* @brief Computes the square root of a scalar floating-point value.
	* 
	* Calculates the square root.
	* 
	* @param scalar The input value.
	* 
	* @return The square root of the input value.
	* @warning Scalar value must be >= 0 for valid result (Input values < 0 will return NaN).
	*/
	NU_FORCEINLINE float SqrtScalar(float scalar)
	{
		return std::sqrt(scalar);
	}

	// =============================================
	// Special Ops
	// =============================================

	/*
	* @brief Computes the horizontal sum of the 4D vector.
	*
	* Efficiently calculates the sum x + y + z + w of a 4D vector.
	* 
	* @param v Input vector.
	* 
	* @return Sum of x, y, z and w components.
	*/
	NU_FORCEINLINE float HorizontalAdd4(const NuVec4& v)
	{
		return v.x + v.y + v.z + v.w;
	}

	/*
	* @brief Computes the horizontal sum of the first three components of a 4D vector.
	* 
	* Efficiently calculates the sum x + y + z of a 4D vector (ignores w component).
	* 
	* @param v Input vector (const ref).
	* 
	* @return Sum of x, y, and z components.
	*/
	NU_FORCEINLINE float HorizontalAdd3(const NuVec4& v)
	{
		return v.x + v.y + v.z;
	}

	/*
	* @brief Computes the horizontal sum of the first two components of a 4D vector.
	* 
	* Efficiently calculates the sum x + y of a 4D vector (ignores z and w components).
	* 
	* @param v Input vector (const ref).
	* 
	* @return Sum of x and y components.
	*/
	NU_FORCEINLINE float HorizontalAdd2(const NuVec4& v)
	{
		return v.x + v.y;
	}

	/*
	* @brief NNormalizes first 2 components (x,y) of 4D vector.
	* 
	* Computes a unit vector from x/y components while leaving z/w unchanged.
	* 
	* @param v Input vector (const ref).
	* 
	* @return Normalized vector with original z/w.
	*/
	NU_FORCEINLINE NuVec4 Normalize2(const NuVec4& v)
	{
		float lengthSquared = v.x * v.x + v.y * v.y;
		if (lengthSquared <= 0.0f)
			return { 0.0f, 0.0f, v.z, v.w };

		float invLength = 1.0f / std::sqrt(lengthSquared);
		return { v.x * invLength, v.y * invLength, v.z, v.w };
	}

	/*
	* @brief Normalizes first 3 components (x,y,z) of 4D vector.
	* 
	* Computes unit vector from x,y,z components while preserving w.
	* 
	* @param v Input vector (const ref).
	* 
	* @return Normalized vector with original w component.
	*/
	NU_FORCEINLINE NuVec4 Normalize3(const NuVec4& v)
	{
		float lengthSquared = v.x * v.x + v.y * v.y + v.z * v.z;
		if (lengthSquared <= 0.0f)
			return { 0.0f, 0.0f, 0.0f, v.w };

		float invLength = 1.0f / std::sqrt(lengthSquared);
		return { v.x * invLength, v.y * invLength, v.z * invLength, v.w };
	}

	/*
	* @brief Normalizes full 4D vector.
	* 
	* Computes unit vector from all components (x,y,z,w).
	* 
	* @param v Input vector (const ref).
	* 
	* @return Fully normalized vector.
	*/
	NU_FORCEINLINE NuVec4 Normalize4(const NuVec4& v)
	{
		float lengthSquared = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
		if (lengthSquared == 0.0f) return { 0, 0, 0, 0 };
		float invLength = 1.0f / std::sqrt(lengthSquared);
		return { v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength };
	}

	// =============================================
	// Extra Math Utilities
	// =============================================

	/*
	* @brief Computes 3D cross product of two vectors (ignores w components).
	* 
	* Calculates the cross product for the first three components (x,y,z), setting w component to 0.
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return Cross product vector {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0}.
	*/
	NU_FORCEINLINE NuVec4 Cross(const NuVec4& a, const NuVec4& b)
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x,
			0.0f
		};
	}

	/*
	* @brief Computes 3D dot product (ignores w components)
	* 
	* Calculates the dot product of the first three components (x,y,z).
	* 
	* @param a First vector.
	* @param b Second vector.
	* 
	* @return Dot product (a.x*b.x + a.y*b.y + a.z*b.z).
	*/
	NU_FORCEINLINE float Dot3(const NuVec4& a, const NuVec4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/*
	* @brief Computes 3D vector length (ignores w component).
	* 
	* Calculates the Euclidean length of the first three components (x,y,z).
	* 
	* @param v Input vector.
	* @return Length of vector (sqrt(x² + y² + z²)).
	*/
	NU_FORCEINLINE float Length3(const NuVec4& v)
	{
		return std::sqrt(Dot3(v, v));
	}


	// =============================================
	// Matricies
	// =============================================

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 SetIdentityMatrix()
	{
		SimdMat4 result{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.mat[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
		return result;
	}

	/*
	* @brief 
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 Transpose(const SimdMat4& m)
	{
		SimdMat4 result{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.mat[i][j] = m.mat[j][i];
			}
		}
		return result;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 Add(const SimdMat4& a, const SimdMat4& b)
	{
		SimdMat4 result{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.mat[i][j] = a.mat[i][j] + b.mat[i][j];
			}
		}
		return result;
	}

	/*
	* @brief 
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 Sub(const SimdMat4& a, const SimdMat4& b)
	{
		SimdMat4 result{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.mat[i][j] = a.mat[i][j] - b.mat[i][j];
			}
		}
		return result;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 Multiply(const SimdMat4& a, const SimdMat4& b)
	{
		SimdMat4 result{};
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				result.mat[row][col] =
					a.mat[row][0] * b.mat[0][col] +
					a.mat[row][1] * b.mat[1][col] +
					a.mat[row][2] * b.mat[2][col] +
					a.mat[row][3] * b.mat[3][col];
			}
		}
		return result;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE NuVec4 Multiply(const SimdMat4& m, const NuVec4& v)
	{
		return {
			m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w,
			m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w,
			m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w,
			m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w,
		};
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 FromRows(const float* row0, const float* row1, const float* row2, const float* row3)
	{
		SimdMat4 result{};
		for (int col = 0; col < 4; ++col)
		{
			result.mat[col][0] = row0[col];
			result.mat[col][1] = row1[col];
			result.mat[col][2] = row2[col];
			result.mat[col][3] = row3[col];
		}
		return result;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 FromColumns(const float* col0, const float* col1, const float* col2, const float* col3)
	{
		SimdMat4 result{};
		for (int row = 0; row < 4; ++row)
		{
			result.mat[0][row] = col0[row];
			result.mat[1][row] = col1[row];
			result.mat[2][row] = col2[row];
			result.mat[3][row] = col3[row];
		}
		return result;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE float Determinant(const SimdMat4& m)
	{
		float det;

		float subFactor00 = m.mat[2][2] * m.mat[3][3] - m.mat[3][2] * m.mat[2][3];
		float subFactor01 = m.mat[2][1] * m.mat[3][3] - m.mat[3][1] * m.mat[2][3];
		float subFactor02 = m.mat[2][1] * m.mat[3][2] - m.mat[3][1] * m.mat[2][2];
		float subFactor03 = m.mat[2][0] * m.mat[3][3] - m.mat[3][0] * m.mat[2][3];
		float subFactor04 = m.mat[2][0] * m.mat[3][2] - m.mat[3][0] * m.mat[2][2];
		float subFactor05 = m.mat[2][0] * m.mat[3][1] - m.mat[3][0] * m.mat[2][1];

		float cof00 = +(m.mat[1][1] * subFactor00 - m.mat[1][2] * subFactor01 + m.mat[1][3] * subFactor02);
		float cof01 = -(m.mat[1][0] * subFactor00 - m.mat[1][2] * subFactor03 + m.mat[1][3] * subFactor04);
		float cof02 = +(m.mat[1][0] * subFactor01 - m.mat[1][1] * subFactor03 + m.mat[1][3] * subFactor05);
		float cof03 = -(m.mat[1][0] * subFactor02 - m.mat[1][1] * subFactor04 + m.mat[1][2] * subFactor05);

		det = m.mat[0][0] * cof00 + m.mat[0][1] * cof01 + m.mat[0][2] * cof02 + m.mat[0][3] * cof03;

		return det;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE SimdMat4 CreateTranslation(const NuVec4& v)
	{
		SimdMat4 result(SetIdentityMatrix());
		result.mat[0][3] = v.x;
		result.mat[1][3] = v.y;
		result.mat[2][3] = v.z;
		return result;
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE NuVec4 GetColumn(const SimdMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return { m.mat[0][index], m.mat[1][index], m.mat[2][index], m.mat[3][index] };
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE NuVec4 GerRow(const SimdMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return { m.mat[index][0], m.mat[index][1], m.mat[index][2], m.mat[index][3] };
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE float Access(const SimdMat4& m, int row, int col)
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		return m.mat[row][col];
	}

	/*
	* @brief
	* 
	* 
	*/
	NU_FORCEINLINE const float* Data(const SimdMat4& m)
	{
		return &m.mat[0][0];
	}
}