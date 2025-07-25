#include "../include/Vector4.hpp"

namespace Engine::Math
{
	Vector4::Vector4() : m_data(Simd::SetZero()) {}
	Vector4::Vector4(float x, float y, float z, float w)
		: m_data(Simd::Set(x, y, z, w))
	{
	}

	Vector4::Vector4(std::initializer_list<float> list)
	{
		assert(list.size() == 4);
		auto it = list.begin();
		m_data = Simd::Set(it[0], it[1], it[2], it[3]);
	}

	Vector4::Vector4(const Vector4& other)
		: m_data(other.m_data)
	{
	}

	Vector4 Vector4::operator+(const Vector4& other) const
	{
		return Vector4(Simd::Add(m_data, other.m_data));
	}

	Vector4 Vector4::operator-(const Vector4& other) const
	{
		return Vector4(Simd::Sub(m_data, other.m_data));
	}

	Vector4 Vector4::operator*(float scalar) const
	{
		return Vector4(Simd::Mul(m_data, Simd::Set(scalar)));
	}

	Vector4 Vector4::operator/(float scalar) const
	{
		return Vector4(Simd::Div(m_data, Simd::Set(scalar)));
	}

	Vector4& Vector4::operator+=(const Vector4& other)
	{
		m_data = Simd::Add(m_data, other.m_data);
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& other)
	{
		m_data = Simd::Sub(m_data, other.m_data);
		return *this;
	}

	Vector4& Vector4::operator*=(float scalar)
	{
		m_data = Simd::Mul(m_data, Simd::Set(scalar));
		return *this;
	}

	Vector4& Vector4::operator/=(float scalar)
	{
		m_data = Simd::Div(m_data, Simd::Set(scalar));
		return *this;
	}

	float Vector4::GetComponent(int index) const {
		switch (index) {
		case 0: return Simd::GetX(m_data);
		case 1: return Simd::GetY(m_data);
		case 2: return Simd::GetZ(m_data);
		case 3: return Simd::GetW(m_data);
		default: assert(false); return 0.0f;
		}
	}

	float Vector4::operator[](int index) const
	{
		return Vector4::GetComponent(index);
	}

	float Vector4::operator[](int index)
	{
		return Vector4::GetComponent(index);
	}

	Vector4 Vector4::operator-() const
	{
		return Vector4(Simd::Mul(m_data, Simd::Set(-1.0f)));
	}

	Vector4& Vector4::operator=(const Vector4& other)
	{
		if (this != &other)
		{
			m_data = other.m_data;
		}
		return *this;
	}

	Vector4& Vector4::operator=(Vector4&& other)
	{
		if (this != &other)
		{
			m_data = std::move(other.m_data);
		}
		return *this;
	}

	bool Vector4::operator==(const Vector4& other) const
	{
		return Simd::Equal(m_data, other.m_data);
	}

	bool Vector4::operator!=(const Vector4& other) const
	{
		return !(*this == other);
	}

	float Vector4::Length() const
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::SqrtScalar(Simd::HorizontalAdd3(mul));
	}

	Vector4 Vector4::Normalize() const
	{
		return Vector4(Simd::Normalize(m_data));
	}

	float Vector4::Dot(const Vector4& other) const
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, other.m_data);
		return Simd::HorizontalAdd3(mul);
	}

	float Vector4::LengthSquared() const
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::HorizontalAdd3(mul);
	}

	const float* Vector4::Data() const
	{
		return reinterpret_cast<const float*>(&m_data);
	}

	const Simd::NuVec4& Vector4::SimdData() const
	{ 
		return m_data; 
	}

	Vector4 Vector4::Zero()
	{
		return Simd::SetZero();
	}

	Vector4 Vector4::One()
	{
		return Simd::Set(1.0f);
	}

	Vector4 Vector4::UnitX()
	{
		return Simd::Set(1.0f, 0.0f, 0.0f, 0.0f);
	}

	Vector4 Vector4::UnitY()
	{
		return Simd::Set(0.0f, 1.0f, 0.0f, 0.0f);
	}

	Vector4 Vector4::UnitZ()
	{
		return Simd::Set(0.0f, 0.0f, 1.0f, 0.0f);
	}

	Vector4 Vector4::UnitW()
	{
		return Simd::Set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	std::string Vector4::ToString() const
	{
		std::ostringstream oss;
		oss << "(" << GetComponent(0) << ", " << GetComponent(1) << ", " << GetComponent(2) << ", " << GetComponent(3) << ")";
		return oss.str();
	}
}