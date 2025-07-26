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
		Vector2(std::initializer_list<float> list) noexcept;
		Vector2(const Vector2& other) noexcept = default;

		[[nodiscard]] Vector2 operator+(const Vector2& other) const noexcept;
		[[nodiscard]] Vector2 operator-(const Vector2& other) const noexcept;
		[[nodiscard]] Vector2 operator*(float scalar) const noexcept;
		[[nodiscard]] Vector2 operator/(float scalar) const noexcept;
		
		Vector2& operator+=(const Vector2& other) noexcept;
		Vector2& operator-=(const Vector2& other) noexcept;
		Vector2& operator*=(float scalar) noexcept;
		Vector2& operator/=(float scalar) noexcept;

		[[nodiscard]] float GetComponent(int) const noexcept;

		[[nodiscard]] bool operator==(const Vector2& other) const noexcept;
		[[nodiscard]] bool operator!=(const Vector2& other) const noexcept;

		[[nodiscard]] float operator[](int) const noexcept;
		[[nodiscard]] float operator[](int) noexcept;
		Vector2& operator=(const Vector2&) noexcept = default;
		Vector2& operator=(Vector2&&) noexcept = default;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float Dot(const Vector2& other) const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		[[nodiscard]] Vector2 Normalize() const noexcept;

		std::string ToString() const;

	private:
		Simd::NuVec4 m_data;

		explicit Vector2(Simd::NuVec4 simd) : m_data(simd) {}
	};
}