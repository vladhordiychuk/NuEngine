#include "../include/Matrix4x4.hpp"

namespace Engine::Math
{
	Matrix4x4::Matrix4x4() noexcept
		: m_data(Simd::SetIdentityMatrix())
	{
	}

	Matrix4x4::Matrix4x4(const Simd::NuMat4& simdData) noexcept
		: m_data(simdData)
	{
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& other) noexcept
		: m_data(other.m_data)
	{
	}

	Matrix4x4::Matrix4x4(Matrix4x4&& other) noexcept
		: m_data(std::move(other.m_data))
	{
	}

	Matrix4x4 Matrix4x4::Identity() noexcept 
	{
		return Matrix4x4(Simd::SetIdentityMatrix());
	}

	Matrix4x4 Matrix4x4::FromRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept
	{
		return Matrix4x4(Simd::FromRows(row0.Data(), row1.Data(), row2.Data(), row3.Data()));
	}

	Matrix4x4 Matrix4x4::FromColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3) noexcept
	{
		return Matrix4x4(Simd::FromColumns(col0.Data(), col1.Data(), col2.Data(), col3.Data()));
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const noexcept
	{
		return Matrix4x4(Simd::Multiply(m_data, other.m_data));
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) noexcept
	{
		m_data = Simd::Multiply(m_data, other.m_data);
		return *this;
	}

	Vector4 Matrix4x4::operator*(const Vector4& other) const noexcept
	{
		return Vector4(Simd::Multiply(m_data, other.SimdData()));
	}

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const noexcept
	{
		return Matrix4x4(Simd::Add(m_data, other.m_data));
	}

	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) noexcept
	{
		m_data = Simd::Add(m_data, other.m_data);
		return *this;
	}

	Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const noexcept
	{
		return Matrix4x4(Simd::Sub(m_data, other.m_data));
	}

	Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) noexcept
	{
		m_data = Simd::Sub(m_data, other.m_data);
		return *this;
	}

	Matrix4x4 Matrix4x4::Transpose() const noexcept
	{
		return Matrix4x4(Simd::Transpose(m_data));
	}

	float Matrix4x4::Determinant() const noexcept
	{
		return Simd::Determinant(m_data);
	}

	Matrix4x4 Matrix4x4::Inverse() const noexcept
	{
		return Matrix4x4(Simd::Inverse(m_data));
	}

	//Matrix4x4 Matrix4x4::CreateRotation(const Quaternion& rotation) noexcept
	//{
	//	return Matrix4x4(Simd::CreateRotation(rotation));
	//}

	Matrix4x4 Matrix4x4::CreateScale(const Vector3& scale) noexcept
	{
		return Matrix4x4(Simd::CreateScale(scale.SimdData()));
	}
	
	Matrix4x4 Matrix4x4::CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept
	{
		return Matrix4x4(Simd::CreateLookAt(eye.SimdData(), target.SimdData(), up.SimdData()));
	}

	Matrix4x4 Matrix4x4::CreatePerspective(float fovY, float aspect, float nearZ, float farZ) noexcept
	{
		return Matrix4x4(Simd::CreatePerspective(fovY, aspect, nearZ, farZ));
	}

	Vector4 Matrix4x4::GetColumn(int index) const noexcept
	{ 
		return Vector4(Simd::GetColumn(m_data, index));
	}

	Vector4 Matrix4x4::GetRow(int index) const noexcept
	{
		return Vector4(Simd::GetRow(m_data, index));
	}

	const float* Matrix4x4::Data() const noexcept
	{
		return Simd::Data(m_data);
	}

	float Matrix4x4::operator()(int row, int col) const noexcept
	{
		return Simd::Access(m_data, row, col);
	}

	bool Matrix4x4::IsIdentity(float epsilon) const noexcept
	{
		return Simd::IsIdentity(m_data, epsilon);
	}

	std::string Matrix4x4::ToString() const
	{
		std::ostringstream oss;
		oss << "Matrix4x4(" << "\n";
		for (int i = 0; i < 4; ++i)
		{
			oss << GetRow(i).ToString() << "\n";
		}
		oss << ")";
		return oss.str();
	}
}
