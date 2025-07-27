#include "../include/Quaternion.hpp"

namespace Engine::Math
{
	Quaternion::Quaternion() noexcept
		: m_data(Simd::Set(0.0f, 0.0f, 0.0f, 0.0f))
	{
	}
	
	Quaternion::Quaternion(float x, float y, float z, float w) noexcept
		: m_data(Simd::Set(x, y, z, w))
	{
	}

	Quaternion::Quaternion(const Vector4& vec) noexcept
		: m_data(Simd::Set(vec.X(), vec.Y(), vec.Z(), vec.W()))
	{
	}

	Quaternion::Quaternion(const Quaternion& other) noexcept
		: m_data(other.m_data)
	{
	}

	Quaternion::Quaternion(std::initializer_list<float> list) noexcept
	{
		assert(list.size() == 4);
		auto it = list.begin();
		m_data = Simd::Set(it[0], it[1], it[2], it[3]);
	}

	Quaternion::Quaternion(Simd::NuVec4 vec) noexcept
		: m_data(vec)
	{
	}

	float Quaternion::X() const noexcept
	{
		return Simd::GetX(m_data);
	}

	float Quaternion::Y() const noexcept
	{
		return Simd::GetY(m_data);
	}

	float Quaternion::Z() const noexcept
	{
		return Simd::GetZ(m_data);
	}

	float Quaternion::W() const noexcept
	{
		return Simd::GetW(m_data);
	}

	const float* Quaternion::Data() const noexcept
	{
		return reinterpret_cast<const float*>(&m_data);
	}

	const Simd::NuVec4& Quaternion::SimdData() const noexcept
	{
		return m_data;
	}

	Quaternion Quaternion::operator+(const Quaternion& other) const noexcept
	{
		return Quaternion(Simd::Add(m_data, other.m_data));
	}

	Quaternion Quaternion::operator-(const Quaternion& other) const noexcept
	{
		return Quaternion(Simd::Sub(m_data, other.m_data));
	}

	Quaternion Quaternion::operator/(const Quaternion& other) const noexcept
	{
		return;
	}
	
	Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
	{
		return;
	}

	Quaternion Quaternion::operator*(float scalar) const noexcept
	{
		return Quaternion(Simd::Mul(m_data, Simd::Set(scalar)));
	}

	Quaternion Quaternion::operator/(float scalar) const noexcept
	{
		return Quaternion(Simd::Div(m_data, Simd::Set(scalar)));
	}

	Quaternion& Quaternion::operator+=(const Quaternion& other) noexcept
	{
		m_data = Simd::Add(m_data, other.m_data);
		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& other) noexcept
	{
		m_data = Simd::Sub(m_data, other.m_data);
		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
	{
		return;
	}

	Quaternion& Quaternion::operator/=(const Quaternion& other) noexcept
	{
		return;
	}

	Quaternion& Quaternion::operator*=(float scalar) noexcept
	{
		m_data = Simd::Mul(m_data, Simd::Set(scalar));
		return *this;
	}

	Quaternion& Quaternion::operator/=(float scalar) noexcept
	{
		m_data = Simd::Div(m_data, Simd::Set(scalar));
		return *this;
	}

	bool Quaternion::operator==(const Quaternion& other) const noexcept
	{
		return Simd::Equal(m_data, other.m_data);
	}

	bool Quaternion::operator!=(const Quaternion& other) const noexcept
	{
		return !(*this == other);
	}



	std::string Quaternion::ToString() const
	{
		std::ostringstream oss;
		oss << "Quaterninon(" << "\n";
		oss << X() << ", " << Y() << ", " << Z() << ", " << W() << ")" << "\n";
		return oss.str();
	}
}
