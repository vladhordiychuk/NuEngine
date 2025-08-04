#pragma once

#include <cmath>
#include <cassert>

namespace Engine::Math::Simd_Scalar
{
	// =============================================
	// Common types
	// =============================================

	struct alignas(16) SimdVec4
	{
		float x, y, z, w;
	};

	struct alignas(16) SimdMat4
	{
		float mat[4][4];
	};

	// =============================================
	// Vectors
	// =============================================

	inline SimdVec4 Set(float x, float y, float z, float w = 0.0f)
	{
		return { x, y, z, w };
	}

	inline SimdVec4 Set(float scalar)
	{
		return { scalar, scalar, scalar, scalar };
	}

	inline SimdVec4 Add(SimdVec4 a, SimdVec4 b)
	{
		return SimdVec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	inline SimdVec4 Sub(SimdVec4 a, SimdVec4 b)
	{
		return SimdVec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	inline SimdVec4 Mul(SimdVec4 a, SimdVec4 b)
	{
		return SimdVec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	inline SimdVec4 Div(SimdVec4 a, SimdVec4 b)
	{
		return SimdVec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
	}

	inline float GetX(SimdVec4 v)
	{
		return v.x;
	}

	inline float GetY(SimdVec4 v)
	{
		return v.y;
	}

	inline float GetZ(SimdVec4 v)
	{
		return v.z;
	}

	inline float GetW(SimdVec4 v)
	{
		return v.w;
	}

	inline float HorizontalAdd3(SimdVec4 v)
	{
		return v.x + v.y + v.z;
	}

	inline float HorizontalAdd2(SimdVec4 v)
	{
		return v.x + v.y;
	}

	inline float HorizontalAdd4(SimdVec4 v)
	{
		return v.x + v.y + v.z + v.w;
	}

	inline SimdVec4 Min(SimdVec4 a, SimdVec4 b)
	{
		return {
			(a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y,
			(a.z < b.z) ? a.z : b.z,
			(a.w < b.w) ? a.w : b.w
		};
	}

	inline SimdVec4 Max(SimdVec4 a, SimdVec4 b)
	{
		return {
			(a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y,
			(a.z > b.z) ? a.z : b.z,
			(a.w > b.w) ? a.w : b.w
		};
	}

	inline SimdVec4 Abs(SimdVec4 v)
	{
		return { std::fabs(v.x), std::fabs(v.y), std::fabs(v.z), std::fabs(v.w) };
	}

	inline bool CompareEqual(SimdVec4 a, SimdVec4 b)
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
	}

	inline float SqrtScalar(float scalar)
	{
		return std::sqrt(scalar);
	}

	inline SimdVec4 SetZero()
	{
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	inline SimdVec4 Normalize(SimdVec4 v)
	{
		float lengthSquared = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
		if (lengthSquared == 0.0f) return { 0, 0, 0, 0 };
		float invLength = 1.0f / std::sqrt(lengthSquared);
		return { v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength };
	}

	inline SimdVec4 Cross(SimdVec4 a, SimdVec4 b)
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x,
			0.0f
		};
	}

	// =============================================
	// Matricies
	// =============================================

	inline SimdMat4 SetIdentityMatrix()
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

	inline SimdMat4 Transpose(const SimdMat4& m)
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

	inline SimdMat4 Add(const SimdMat4& a, const SimdMat4& b)
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

	inline SimdMat4 Sub(const SimdMat4& a, const SimdMat4& b)
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

	inline SimdMat4 Multiply(const SimdMat4& a, const SimdMat4& b)
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

	inline SimdVec4 Multiply(const SimdMat4& m, const SimdVec4& v)
	{
		return {
			m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w,
			m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w,
			m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w,
			m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w,
		};
	}

	inline SimdMat4 FromRows(const float* row0, const float* row1, const float* row2, const float* row3)
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

	inline SimdMat4 FromColumns(const float* col0, const float* col1, const float* col2, const float* col3)
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

	inline float Determinant(const SimdMat4& m)
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

	inline SimdMat4 Inverse(const SimdMat4& mat)
	{

	}

	inline SimdMat4 CreateTranslation(const SimdVec4& v)
	{
		SimdMat4 result(SetIdentityMatrix());
		result.mat[0][3] = v.x;
		result.mat[1][3] = v.y;
		result.mat[2][3] = v.z;
		return result;
	}



	inline SimdVec4 GetColumn(const SimdMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return { m.mat[0][index], m.mat[1][index], m.mat[2][index], m.mat[3][index] };
	}

	inline SimdVec4 GerRow(const SimdMat4& m, int index)
	{
		assert(index >= 0 && index < 4);
		return { m.mat[index][0], m.mat[index][1], m.mat[index][2], m.mat[index][3] };
	}

	inline float Access(const SimdMat4& m, int row, int col)
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		return m.mat[row][col];
	}

	inline const float* Data(const SimdMat4& m)
	{
		return &m.mat[0][0];
	}
}