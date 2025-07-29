#include "../include/Vector4.hpp"

namespace Engine::Math
{
	Vector4::Vector4() noexcept
		: m_data(Simd::SetZero()) 
	{
	}

	Vector4::Vector4(float x, float y, float z, float w) noexcept
		: m_data(Simd::Set(x, y, z, w))
	{
	}

	Vector4::Vector4(std::initializer_list<float> list)
	{
		assert(list.size() == 4);
		auto it = list.begin();
		m_data = Simd::Set(it[0], it[1], it[2], it[3]);
	}

	Vector4::Vector4(const Vector4& other) noexcept
		: m_data(other.m_data)
	{
	}

	Vector4::Vector4(Vector4&& other)
		: m_data(std::move(other.m_data))
	{
	}

	Vector4 Vector4::operator+(const Vector4& other) const noexcept
	{
		return Vector4(Simd::Add(m_data, other.m_data));
	}

	Vector4 Vector4::operator-(const Vector4& other) const noexcept
	{
		return Vector4(Simd::Sub(m_data, other.m_data));
	}

	Vector4 Vector4::operator*(const Vector4& other) const noexcept
	{
		return Vector4(Simd::Mul(m_data, other.m_data));
	}

	Vector4 Vector4::operator*(float scalar) const noexcept
	{
		return Vector4(Simd::Mul(m_data, Simd::Set(scalar)));
	}

	Vector4 Vector4::operator/(float scalar) const noexcept
	{
		if (std::abs(scalar) < std::numeric_limits<float>::epsilon()) {
			throw std::invalid_argument("Division by zero");
			return Vector4::Zero();
		}
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

	Vector4& Vector4::operator*=(const Vector4& other)
	{
		m_data = Simd::Mul(m_data, other.m_data);
		return *this;
	}

	Vector4& Vector4::operator*=(float scalar)
	{
		m_data = Simd::Mul(m_data, Simd::Set(scalar));
		return *this;
	}

	Vector4& Vector4::operator/=(float scalar)
	{
		if (std::abs(scalar) < std::numeric_limits<float>::epsilon()) {
			throw std::invalid_argument("Division by zero");
			m_data = Simd::SetZero();
			return *this;
		}
		m_data = Simd::Div(m_data, Simd::Set(scalar));
		return *this;
	}

	float Vector4::GetComponent(int index) const
	{
		switch (index) 
		{
		case 0: return Simd::GetX(m_data);
		case 1: return Simd::GetY(m_data);
		case 2: return Simd::GetZ(m_data);
		case 3: return Simd::GetW(m_data);
		default: throw std::out_of_range("Vector4 component index out of range");
		}
	}

	float Vector4::X() const noexcept
	{
		return Simd::GetX(m_data);
	}
	float Vector4::Y() const noexcept
	{
		return Simd::GetY(m_data);
	}

	float Vector4::Z() const noexcept
	{
		return Simd::GetZ(m_data);
	}

	float Vector4::W() const noexcept
	{
		return Simd::GetW(m_data);
	}

	float Vector4::operator[](int index) const
	{
		return Vector4::GetComponent(index);
	}

	Vector4 Vector4::operator-() const noexcept
	{
		return Vector4(Simd::Neg(m_data));
	}

	Vector4& Vector4::operator=(const Vector4& other) noexcept
	{
		if (this != &other)
		{
			m_data = other.m_data;
		}
		return *this;
	}

	Vector4& Vector4::operator=(Vector4&& other) noexcept
	{
		if (this != &other)
		{
			m_data = std::move(other.m_data);
		}
		return *this;
	}

	bool Vector4::operator==(const Vector4& other) const noexcept
	{
		return Simd::Equal(m_data, other.m_data);
	}

	bool Vector4::operator!=(const Vector4& other) const noexcept
	{
		return !(*this == other);
	}

	float Vector4::Length() const noexcept
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::SqrtScalar(Simd::HorizontalAdd4(mul));
	}

	Vector4 Vector4::Normalize() const noexcept
	{
		return Vector4(Simd::Normalize4(m_data));
	}

	float Vector4::Dot(const Vector4& other) const noexcept
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, other.m_data);
		return Simd::HorizontalAdd4(mul);
	}

	float Vector4::LengthSquared() const noexcept
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::HorizontalAdd4(mul);
	}

	float Vector4::Distance(const Vector4& other) const noexcept
	{
		return (*this - other).Length();
	}

	const float* Vector4::Data() const noexcept
	{
		return reinterpret_cast<const float*>(&m_data);
	}

	float* Vector4::Data() noexcept 
	{
		return reinterpret_cast<float*>(&m_data);
	}

	const Simd::NuVec4& Vector4::SimdData() const noexcept
	{ 
		return m_data; 
	}

	Vector4 Vector4::Zero() noexcept
	{
		return Vector4(Simd::SetZero());
	}

	Vector4 Vector4::One() noexcept
	{
		return Vector4(Simd::Set(1.0f, 1.0f, 1.0f, 1.0f));
	}

	Vector4 Vector4::UnitX() noexcept
	{
		return Vector4(Simd::Set(1.0f, 0.0f, 0.0f, 0.0f));
	}

	Vector4 Vector4::UnitY() noexcept
	{
		return Vector4(Simd::Set(0.0f, 1.0f, 0.0f, 0.0f));
	}

	Vector4 Vector4::UnitZ() noexcept
	{
		return Vector4(Simd::Set(0.0f, 0.0f, 1.0f, 0.0f));
	}

	Vector4 Vector4::UnitW() noexcept
	{
		return Vector4(Simd::Set(0.0f, 0.0f, 0.0f, 1.0f));
	}

	std::string Vector4::ToString() const
	{
		std::ostringstream oss;
		oss << "(" << GetComponent(0) << ", " << GetComponent(1) << ", " << GetComponent(2) << ", " << GetComponent(3) << ")";
		return oss.str();
	}

	std::ostream& operator<<(std::ostream& os, const Vector4& vec)
	{
		os << vec.ToString();
		return os;
	}
}