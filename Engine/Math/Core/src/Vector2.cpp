#include "../include/Vector2.hpp"

namespace Engine::Math
{
	Vector2::Vector2() : m_data(Simd::SetZero()) noexcept
	{
	}

	Vector2::Vector2(float x, float y) noexcept  
		: m_data(Simd::Set(x, y, 0.0f, 0.0f))
	{
	}

	Vector2::Vector2(std::initializer_list<float> list) noexcept
	{
		assert(list.size() == 2);
		auto it = list.begin();
		float x = *it++;
		float y = *it++;
		m_data = Simd::Set(x, y, 0.0f, 0.0f);
	}

	Vector2::Vector2(const Vector2& other) noexcept
		: m_data(other.m_data)
	{
	}

	Vector2 Vector2::operator+(const Vector2& other) const noexcept
	{
		return Vector2(Simd::Add(m_data, other.m_data));
	}

	Vector2 Vector2::operator-(const Vector2& other) const noexcept
	{
		return Vector2(Simd::Sub(m_data, other.m_data));
	}

	Vector2 Vector2::operator*(const Vector2& other) const noexcept
	{
		return Vector2(Simd::Mul(m_data, other.m_data));
	}

	Vector2 Vector2::operator*(float scalar) const noexcept
	{
		return Vector2(Simd::Mul(m_data, Simd::Set(scalar)));
	}

	Vector2 Vector2::operator/(float scalar) const noexcept
	{
		return Vector2(Simd::Div(m_data, Simd::Set(scalar)));
	}

	Vector2& Vector2::operator+=(const Vector2& other) noexcept
	{
		m_data = Simd::Add(m_data, other.m_data);
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other) noexcept
	{
		m_data = Simd::Sub(m_data, other.m_data);
		return *this;
	}

	Vector2& Vector2::operator*=(const Vector2& other) noexcept
	{
		m_data = Simd::Mul(m_data, other.m_data);
		return *this;
	}

	Vector2& Vector2::operator*=(float scalar) noexcept
	{
		m_data = Simd::Mul(m_data, Simd::Set(scalar));
		return *this;
	}

	Vector2& Vector2::operator/=(float scalar) noexcept
	{
		m_data = Simd::Div(m_data, Simd::Set(scalar));
		return *this;
	}

	float Vector2::X() const noexcept
	{
		return Simd::GetX(m_data);
	}

	float Vector2::Y() const noexcept
	{
		return Simd::GetY(m_data);
	}

	bool Vector2::operator==(const Vector2& other) const noexcept
	{
		__m128 cmp = _mm_cmpeq_ps(m_data, other.m_data);
		return _mm_movemask_ps(cmp) == 0xF;
	}

	bool Vector2::operator!=(const Vector2& other) const noexcept
	{
		return !(*this == other);
	}

	float Vector2::Length() const noexcept
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::SqrtScalar(Simd::HorizontalAdd2(mul));
	}

	float Vector2::Dot(const Vector2& other) const noexcept
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, other.m_data);
		float dot = Simd::HorizontalAdd2(mul);
		return dot;
	}

	Vector2 Vector2::Normalize() const noexcept
	{
		return Vector2(Simd::Normalize2(m_data));
	}

	float Vector2::LengthSquared() const noexcept
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::HorizontalAdd2(mul);
	}

	float Vector2::Distance(const Vector2& other) noexcept
	{
		return (*this - other).Length();
	}

	Vector2& Vector2::operator=(const Vector2& other) noexcept
	{
		if (this != &other)
		{
			m_data = other.m_data;
		}
		return *this;
	}

	Vector2& Vector2::operator=(Vector2&& other) noexcept
	{
		if (this != &other)
		{
			m_data = std::move(other.m_data);
		}
		return *this;
	}

	float Vector2::operator[](int index) const noexcept
	{
		return GetComponent(index);
	}

	float Vector2::GetComponent(int index) const noexcept
	{
		switch (index)
		{
		case 0: return Simd::GetX(m_data);
		case 1:return Simd::GetY(m_data);
		default: assert(false); return 1.0f;
		}
	}

	Vector2 Vector2::Zero() noexcept
	{
		return Vector2(Simd::SetZero());
	}

	Vector2 Vector2::One() noexcept
	{
		return Vector2(Simd::Set(1.0f, 1.0f));
	}

	Vector2 Vector2::UnitX() noexcept
	{
		return Vector2(Simd::Set(1.0f, 0.0f));
	}

	Vector2 Vector2::UnitY() noexcept
	{
		return Vector2(Simd::Set(0.0f, 1.0f));
	}

	std::string Vector2::ToString() const
	{
		std::ostringstream oss;
		oss << "(" << X() << ", " << Y() << ")";
		return oss.str();
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vec)
	{
		os << vec.ToString();
		return os;
	}
}