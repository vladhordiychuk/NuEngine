#include "../include/Vector2.hpp"

namespace Engine::Math
{
	Vector2::Vector2() : m_data(Simd::SetZero())
	{
	}

	Vector2::Vector2(float x, float y) :
		m_data(Simd::Set(x, y, 0, 0))
	{
	}

	Vector2::Vector2(std::initializer_list<float> list)
	{
		assert(list.size() == 2);
		auto it = list.begin();
		m_data = Simd::Set(it[0], it[1], 0, 0);
	}

	Vector2::Vector2(const Vector2& other)
		: m_data(other.m_data)
	{
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(Simd::Add(m_data, other.m_data));
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(Simd::Sub(m_data, other.m_data));
	}

	Vector2 Vector2::operator*(float scalar) const
	{
		return Vector2(Simd::Mul(m_data, Simd::Set(scalar)));
	}

	Vector2 Vector2::operator/(float scalar) const
	{
		return Vector2(Simd::Div(m_data, Simd::Set(scalar)));
	}

	Vector2& Vector2::operator+=(const Vector2& other)
	{
		m_data = Simd::Add(m_data, other.m_data);
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other)
	{
		m_data = Simd::Sub(m_data, other.m_data);
		return *this;
	}

	Vector2& Vector2::operator*=(float scalar)
	{
		m_data = Simd::Mul(m_data, Simd::Set(scalar));
		return *this;
	}

	Vector2& Vector2::operator/=(float scalar)
	{
		m_data = Simd::Div(m_data, Simd::Set(scalar));
		return *this;
	}

	float Vector2::X() const
	{
		return Simd::GetX(m_data);
	}

	float Vector2::Y() const
	{
		return Simd::GetY(m_data);
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		__m128 cmp = _mm_cmpeq_ps(m_data, other.m_data);
		return _mm_movemask_ps(cmp) == 0xF;
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	float Vector2::Length() const
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::SqrtScalar(Simd::HorizontalAdd2(mul));
	}

	float Vector2::Dot(const Vector2& other) const
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, other.m_data);
		float dot = Simd::HorizontalAdd2(mul);
		return dot;
	}

	Vector2 Vector2::Normalize() const
	{
		return Vector2(Simd::Normalize(m_data));
	}

	float Vector2::LengthSquared() const
	{
		Simd::NuVec4 mul = Simd::Mul(m_data, m_data);
		return Simd::HorizontalAdd2(mul);
	}

	std::string Vector2::ToString() const
	{
		std::ostringstream oss;
		oss << "(" << X() << ", " << Y() << ")";
		return oss.str();
	}
}