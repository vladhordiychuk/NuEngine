// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cmath>
#include <cassert>

#include <Math/Algebra/Vector/VectorAPI.hpp>
#include <Math/Algebra/Matrix/MatrixAPI.hpp>

namespace NuEngine::Math::Simd_Scalar
{

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

	[[nodiscard]] NU_FORCEINLINE NuVec4 Set(float x, float y, float z, float w = 0.0f) noexcept
	{
		return { x, y, z, w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 SetAll(float scalar) noexcept
	{
		return { scalar, scalar, scalar, scalar };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 SetZero() noexcept
	{
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	[[nodiscard]] NU_FORCEINLINE float GetX(const NuVec4& v) noexcept
	{
		return v.x;
	}

	[[nodiscard]] NU_FORCEINLINE float GetY(const NuVec4& v) noexcept
	{
		return v.y;
	}

	[[nodiscard]] NU_FORCEINLINE float GetZ(const  NuVec4& v) noexcept
	{
		return v.z;
	}

	[[nodiscard]] NU_FORCEINLINE float GetW(const NuVec4& v) noexcept
	{
		return v.w;
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Add(const NuVec4& a, const NuVec4& b) noexcept
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Sub(const NuVec4& a, const NuVec4& b) noexcept
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuVec4& a, const NuVec4& b) noexcept
	{ 
		return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Div(const NuVec4& a, const NuVec4& b) noexcept
	{
		return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Neg(const NuVec4& a) noexcept
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Min(const NuVec4& a, const NuVec4& b) noexcept
	{
		return {
			(a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y,
			(a.z < b.z) ? a.z : b.z,
			(a.w < b.w) ? a.w : b.w
		};
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Max(const NuVec4& a, const NuVec4& b) noexcept
	{
		return {
			(a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y,
			(a.z > b.z) ? a.z : b.z,
			(a.w > b.w) ? a.w : b.w
		};
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Abs(const NuVec4& v) noexcept
	{
		return { std::fabs(v.x), std::fabs(v.y), std::fabs(v.z), std::fabs(v.w) };
	}

	[[nodiscard]] NU_FORCEINLINE bool Equal(const NuVec4& a, const NuVec4& b) noexcept
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
	}

	[[nodiscard]] NU_FORCEINLINE float SqrtScalar(float scalar) noexcept
	{ 
		return std::sqrt(scalar);
	}

	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd4(const NuVec4& v) noexcept
	{
		return v.x + v.y + v.z + v.w;
	}

	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd3(const NuVec4& v) noexcept
	{
		return v.x + v.y + v.z;
	}

	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd2(const NuVec4& v) noexcept
	{
		return v.x + v.y;
	}

	NU_FORCEINLINE float fast_inverse_sqrt(float x) noexcept 
	{
		const float xhalf = 0.5f * x;
		int i = *(int*)&x;          
		i = 0x5f3759df - (i >> 1);  
		x = *(float*)&i;            
		return x * (1.5f - xhalf * x * x); 
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize2(const NuVec4& v) noexcept
	{
		float lenSq = v.x * v.x + v.y * v.y;
		assert(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
		float invLen = fast_inverse_sqrt(lenSq);
		return { v.x * invLen, v.y * invLen, v.z, v.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize3(const NuVec4& v) noexcept
	{
		const float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
		assert(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
		const float invLen = fast_inverse_sqrt(lenSq);
		return { v.x * invLen, v.y * invLen, v.z * invLen, v.w };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize4(const NuVec4& v) noexcept
	{
		float lenSq = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
		assert(lenSq > 1e-8f && "Cannot normalize vector with near-zero length!");
		float invLength = fast_inverse_sqrt(lenSq);
		return { v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength };
	}

	[[nodiscard]] NU_FORCEINLINE NuVec4 Cross(const NuVec4& a, const NuVec4& b) noexcept
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x,
			0.0f
		};
	}

	[[nodiscard]] NU_FORCEINLINE float Dot3(const NuVec4& a, const NuVec4& b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	[[nodiscard]] NU_FORCEINLINE float Length3(const NuVec4& v) noexcept
	{
		return std::sqrt(Dot3(v, v));
	}

	// =============================================
	// Matricies
	// =============================================

	NU_FORCEINLINE SimdMat4 SetIdentityMatrix() noexcept
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

	NU_FORCEINLINE SimdMat4 Transpose(const SimdMat4& m) noexcept
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

	NU_FORCEINLINE SimdMat4 Add(const SimdMat4& a, const SimdMat4& b) noexcept
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

	NU_FORCEINLINE SimdMat4 Sub(const SimdMat4& a, const SimdMat4& b) noexcept
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

	NU_FORCEINLINE SimdMat4 Multiply(const SimdMat4& a, const SimdMat4& b) noexcept
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

	NU_FORCEINLINE NuVec4 Multiply(const SimdMat4& m, const NuVec4& v) noexcept
	{
		return {
			m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w,
			m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w,
			m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w,
			m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w,
		};
	}

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

	NU_FORCEINLINE SimdMat4 CreateTranslation(const NuVec4& v)
	{
		SimdMat4 result(SetIdentityMatrix());
		result.mat[0][3] = v.x;
		result.mat[1][3] = v.y;
		result.mat[2][3] = v.z;
		return result;
	}

	NU_FORCEINLINE NuVec4 GetColumn(const SimdMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return { m.mat[0][index], m.mat[1][index], m.mat[2][index], m.mat[3][index] };
	}

	NU_FORCEINLINE NuVec4 GerRow(const SimdMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return { m.mat[index][0], m.mat[index][1], m.mat[index][2], m.mat[index][3] };
	}

	NU_FORCEINLINE float Access(const SimdMat4& m, int row, int col)
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		return m.mat[row][col];
	}

	NU_FORCEINLINE const float* Data(const SimdMat4& m)
	{
		return &m.mat[0][0];
	}
}