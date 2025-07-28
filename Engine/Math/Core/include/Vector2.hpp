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
		Vector2() noexcept = default;
		Vector2(float x, float y) noexcept;
		Vector2(std::initializer_list<float> list);
		Vector2(const Vector2&) noexcept = default;

		[[nodiscard]] Vector2 operator+(const Vector2&) const noexcept;
		[[nodiscard]] Vector2 operator-(const Vector2&) const noexcept;
		[[nodiscard]] Vector2 operator*(const Vector2&) const noexcept;
		[[nodiscard]] Vector2 operator*(float) const noexcept;
		[[nodiscard]] Vector2 operator/(float) const noexcept;
		
		Vector2& operator+=(const Vector2&) noexcept;
		Vector2& operator-=(const Vector2&) noexcept;
		Vector2& operator*(const Vector2&) noexcept;
		Vector2& operator*=(float) noexcept;
		Vector2& operator/=(float) noexcept;

		[[nodiscard]] float GetComponent(int) const;
		[[nodiscard]] float X() const noexcept;
		[[nodiscard]] float Y() const noexcept;

		[[nodiscard]] bool operator==(const Vector2&) const noexcept;
		[[nodiscard]] bool operator!=(const Vector2&) const noexcept;

		[[nodiscard]] static Vector2 Zero() noexcept;
		[[nodiscard]] static Vector2 One() noexcept;
		[[nodiscard]] static Vector2 UnitX() noexcept;
		[[nodiscard]] static Vector2 UnitY() noexcept;

		[[nodiscard]] float operator[](int) const noexcept;
		Vector2& operator=(const Vector2&) noexcept = default;
		Vector2& operator=(Vector2&&) noexcept = default;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float Dot(const Vector2&) const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;
		[[nodiscard]] float Distance(const Vector2&) const noexcept;

		[[nodiscard]] const float* Data() const noexcept;
		[[nodiscard]] const Simd::NuVec4& SimdData() const noexcept;

		[[nodiscard]] Vector2 Normalize() const noexcept;

		std::string ToString() const;

		friend std::ostream& operator<<(std::ostream&, const Vector2&);

	private:
		Simd::NuVec4 m_data;

		explicit Vector2(Simd::NuVec4 simd) : m_data(simd) {}
	};
}