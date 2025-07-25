#include "../include/Matrix4x4.hpp"

namespace Engine::Math
{
	Matrix4x4::Matrix4x4()
		: m_data(Simd::SetIdentityMatrix())
	{
	}

	Matrix4x4::Matrix4x4(const Simd::NuMat4& simdData)
		: m_data(simdData)
	{
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& other)
		: m_data(other.m_data)
	{
	}

	Matrix4x4::Matrix4x4(const Matrix4x4&& other)
		: m_data(std::move(other.m_data))
	{
	}

	Matrix4x4 Matrix4x4::Identity() noexcept
	{
		return Simd::SetIdentityMatrix();
	}

	Matrix4x4 Matrix4x4::FromRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
	{
		return Simd::FromRows(row0.Data(), row1.Data(), row2.Data(), row3.Data());
	}

	Matrix4x4 Matrix4x4::FromColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3)
	{
		return Simd::FromColumns(col0.Data(), col1.Data(), col2.Data(), col3.Data());
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
	{
		return Simd::Multiply(m_data, other.m_data);
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other)
	{
		m_data = Simd::Multiply(m_data, other.m_data);
		return *this;
	}

	Vector4 Matrix4x4::operator*(const Vector4& other) const
	{
		return Simd::Multiply(m_data, other.SimdData());
	}

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
	{
		return Simd::Add(m_data, other.m_data);
	}

	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other)
	{
		m_data = Simd::Add(m_data, other.m_data);
		return *this;
	}

	Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
	{
		return Simd::Sub(m_data, other.m_data);
	}

	Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other)
	{
		m_data = Simd::Sub(m_data, other.m_data);
		return *this;
	}

	Matrix4x4 Matrix4x4::Transpose() const
	{
		return Simd::Transpose(m_data);
	}

	float Matrix4x4::Determinant() const
	{
		return Simd::Determinant(m_data);
	}

	Matrix4x4 Matrix4x4::Inverse() const
	{
		return Simd::Inverse(m_data);
	}

	Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& translation)
	{
		return Simd::CreateTranslation(translation.SimdData());
	}

	Matrix4x4 Matrix4x4::CreateRotation(const Quaternion& rotation)
	{
		return Simd::CreateRotation(rotation);
	}

	Matrix4x4 Matrix4x4::CreateScale(const Vector3& scale)
	{
		return Simd::CreateScale(scale.SimdData());
	}
	
	Matrix4x4 Matrix4x4::CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		return Simd::CreateLookAt(eye.SimdData(), target.SimdData(), up.SimdData());
	}

	Matrix4x4 Matrix4x4::CreatePerspective(float fovY, float aspect, float nearZ, float farZ)
	{
		return Simd::CreatePerspective(fovY, aspect, nearZ, farZ);
	}

	Matrix4x4 Matrix4x4::CreateOrthographic(float left, float right, float bottom, float top, float nearZ, float farZ)
	{
		return Simd::CreateOrthographic(left, right, bottom, nearZ, farZ);
	}

	void Matrix4x4::Decompose(Vector3& traslation, Quaternion& rotation, Vector3& scale) const
	{
		return Simd::Decompose(m_data, traslation.SimdData(), rotation.SimdData(), scale.SimdData());
	}

	Vector4 Matrix4x4::GetColumn(int index) const
	{
		return Simd::GetColumn(m_data, index);
	}

	Vector4 Matrix4x4::GetRow(int index) const
	{
		return Simd::GetRow(m_data, index);
	}

	const float* Matrix4x4::Data() const
	{
		return Simd::Data(m_data);
	}

	float Matrix4x4::operator()(int row, int col) const
	{
		return Simd::Access(m_data, row, col);
	}

	bool Matrix4x4::IsIdentity(float epsilon) const
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
