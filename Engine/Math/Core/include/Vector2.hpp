#pragma once

#include "../../SIMD/SIMDBackend.hpp"

#include <string>
#include <initializer_list>
#include <sstream>

namespace Engine::Math
{
	class alignas(16) Vector2
	{
	public:
		Vector2();
		Vector2(float x, float y);
		Vector2(std::initializer_list<float> list);
		Vector2(const Vector2& other);

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(float scalar) const;
		Vector2 operator/(float scalar) const;
		
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(float scalar);
		Vector2& operator/=(float scalar);

		float X() const;
		float Y() const;

		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		float Length() const;
		float Dot(const Vector2& other) const;
		float LengthSquared() const;

		Vector2 Normalize() const;

		std::string ToString() const;

	private:
		Simd::NuVec4 m_data;

		explicit Vector2(Simd::NuVec4 simd) : m_data(simd) {}
	};
}