// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Algebra/Vector/VectorAPI.hpp>

#include <string>
#include <initializer_list>
#include <sstream>

namespace NuEngine::Math
{
	/*
	* @brief 2D Vector with SIMD support.
	*
	* This class implements a 2-component vector with support for arithmetic operations,
	* component access, normalization, length, and distance. It supports SSE optimizations.
	*/
	class alignas(16) Vector2
	{
	public:
		static constexpr int Dimensions = 2;

		/*
		* @brief Creates a zero vector (0.0f, 0.0f).
		*/
		NU_FORCEINLINE Vector2() noexcept
			: m_data(VectorAPI::SetZero())
		{
		}

		/*
		* @brief Creates a vector with specified components.
		* 
		* @param x Value on X.
		* @param y Value on Y.
		*/
		NU_FORCEINLINE Vector2(float x, float y) noexcept
			: m_data(VectorAPI::Set(x, y, 0.0f, 0.0f))
		{
		}

		/*
		* @brief Initialization with initializer_list.
		* 
		* @param Initialization list of 2 elements.
		*/
		NU_FORCEINLINE Vector2(std::initializer_list<float> list) noexcept
		{
			assert(list.size() == 2);
			auto it = list.begin();
			float x = *it++;
			float y = *it++;
			m_data = VectorAPI::Set(x, y, 0.0f, 0.0f);
		}

		/*
		* @brief Constructs a copy of another Vector2.
		*
		* Initializes this vector by copying all components from the given vector.
		*
		* @param other The vector to copy.
		*/
		NU_FORCEINLINE Vector2(const Vector2& other) noexcept
			: m_data(other.m_data)
		{
		}

		/*
		* @brief Constructs a Vector2 by moving another one.
		*
		* Transfers the internal data from the given vector to this one.
		* The source vector is left in a valid but unspecified state.
		*
		* @param other The vector to move from.
		*/
		NU_FORCEINLINE Vector2(Vector2&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}


		/*
		* @brief Constructs a Vector2 from a SIMD vector.
		*
		* Initializes the Vector3 using the given SIMD vector without performing
		* any additional computations or conversions.
		*
		* @param simd The SIMD vector (NuVec4) used to initialize this Vector2.
		*/
		explicit NU_FORCEINLINE Vector2(VectorAPI::NuVec4 vec) noexcept
			: m_data(vec) 
		{
		}

		/*
		* @brief Adding another vector to this one.
		*
		* Performs component-wise addition: each component of another vector is added to the corresponding component of this vector.
		*
		* @param other The vector to add.
		* @return A new Vector2 that is the result of the addition.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator+(Vector2 other) const noexcept
		{
			return Vector2(VectorAPI::Add(m_data, other.m_data));
		}

		/*
		* @brief Subtracting another vector from this one.
		*
		* Performs component-wise subtraction: each component of another vector is subtracted to the corresponding component of this vector.
		*
		* @param other The vector to subtract.
		* @return A new Vector2 that is the result of the subtraction.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator-(Vector2 other) const noexcept
		{
			return Vector2(VectorAPI::Sub(m_data, other.m_data));
		}

		/*
		* @brief Multiplying this vector by another.
		*
		* Performs component-wise multiplication: each component of the other vector
		* is multiplied by the corresponding component of this vector.
		*
		* @param other The vector to multiply with.
		* @return A new Vector2 that is the result of the multiplication.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator*(Vector2 other) const noexcept
		{
			return Vector2(VectorAPI::Mul(m_data, other.m_data));
		}

		/*
		* 
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator/(Vector2 other) const noexcept
		{
			return Vector2(VectorAPI::Div(m_data, other.m_data));
		}

		/*
		* @brief Multiplies the vector by a scalar.
		*
		* Scales each component of the vector by the given scalar value.
		*
		* @param scalar The scalar value to multiply by.
		* @return A new Vector2 that is the result of the multiplication.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator*(float scalar) const noexcept
		{
			return Vector2(VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar)));
		}

		/*
		* @brief Divides the vector by a scalar.
		*
		* Scales each component of the vector by the reciprocal of the given scalar value.
		*
		* @param scalar The scalar value to divide by.
		* @return A new Vector2 that is the result of the division.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator/(float scalar) const noexcept
		{
			assert(std::fabs(scalar) > std::numeric_limits<float>::epsilon() && "Division by zero or near zero!");
			return Vector2(VectorAPI::Div(m_data, VectorAPI::SetAll(scalar)));
		}
		
		/*
		* @brief Adds another vector to this one.
		*
		* Performs component-wise addition and updates the current vector.
		*
		* @param other The vector to add.
		* @return A reference to this vector after the addition.
		*/
		NU_FORCEINLINE Vector2& operator+=(Vector2 other) noexcept
		{
			m_data = VectorAPI::Add(m_data, other.m_data);
			return *this;
		}

		/*
		* @brief Subtracts another vector to this one.
		*
		* Performs component-wise subtraction and updates the current vector.
		*
		* @param other The vector to subtract.
		*/
		NU_FORCEINLINE Vector2& operator-=(Vector2 other) noexcept
		{
			m_data = VectorAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/*
		* @brief Multiplies another vector to this one.
		*
		* Performs component-wise multiplication and updates the current vector.
		*
		* @param other The vector to multiply.
		* @return A reference to this vector after the multiplication.
		*/
		NU_FORCEINLINE Vector2& operator*=(Vector2 other) noexcept
		{
			m_data = VectorAPI::Mul(m_data, other.m_data);
			return *this;
		}

		NU_FORCEINLINE Vector2& operator/=(Vector2 other) noexcept
		{
			m_data = VectorAPI::Div(m_data, other.m_data);
			return *this;
		}

		/*
		* @brief Multiplies this vector by a scalar value.
		*
		* Each component (x, y) will be multiplied by the given scalar.
		*
		* @param scalar The scalar value to multiply the vector by.
		* @return A reference to this vector after the multiplication.
		*/
		NU_FORCEINLINE Vector2& operator*=(float scalar) noexcept
		{
			m_data = VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar));
			return *this;
		}

		/*
		* @brief Divides this vector by a scalar value.
		*
		* Each component (x, y) will be divided by the given scalar.
		*
		* @param scalar The scalar value to divide the vector by.
		* @return A reference to this vector after the division.
		*/
		NU_FORCEINLINE Vector2& operator/=(float scalar) noexcept
		{
			assert(std::fabs(scalar) > std::numeric_limits<float>::epsilon() && "Division by zero or near zero!");
			m_data = VectorAPI::Div(m_data, VectorAPI::SetAll(scalar));
			return *this;
		}

		/*
		* @brief Returns the x-component
		*/
		[[nodiscard]] NU_FORCEINLINE float X() const noexcept
		{
			return VectorAPI::GetX(m_data);
		}


		/*
		* @brief Returns the y-component
		*/
		[[nodiscard]] NU_FORCEINLINE float Y() const noexcept
		{
			return VectorAPI::GetY(m_data);
		}

		/*
		* @brief Compares two Vector2 instances for equality.
		*
		* @param other The vector to compare with.
		* @result true if all components are equal, false otherwise.
		*/
		[[nodiscard]] NU_FORCEINLINE bool operator==(Vector2 other) const noexcept
		{
			return VectorAPI::Equal(m_data, other.m_data);
		}

		/*
		* @brief Compares two Vector2 instances for inequality.
		*
		* @param other The vector to compare with.
		* @result true if any component differs, false otherwise.
		*/
		[[nodiscard]] NU_FORCEINLINE bool operator!=(Vector2 other) const noexcept
		{
			return !(*this == other);
		}

		/*
        * @brief Returns a negated copy of the vector.
        *
        * Each component of the resulting vector is the negation of the corresponding component in this vector.
        *
        * @return A negated vector.
        */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator-() const noexcept
		{
			return Vector2(VectorAPI::Neg(m_data));
		}

		/*
		* @brief Returns a zero vector.
		*
		* The resulting vector has components (0, 0).
		*
		* @return A vector with all components set to 0.
		*/
		[[nodiscard]] NU_FORCEINLINE static Vector2 Zero() noexcept
		{
			return Vector2(VectorAPI::SetZero());
		}

		/*
		* @brief Returns a vector with all components set to 1.
		*
		* The resulting vector has components (1, 1).
		*
		* @return A vector with all components set to 1.
		*/
		[[nodiscard]] NU_FORCEINLINE static Vector2 One() noexcept
		{
			return Vector2(VectorAPI::Set(1.0f, 1.0f, 0.0f, 0.0f));
		}

		/*
		* @brief Returns a unit vector pointing in the X direction.
		*
		* The resulting vector has components (1, 0).
		*
		* @return A unit vector with only the X component set to 1.
		*/
		[[nodiscard]] NU_FORCEINLINE static Vector2 UnitX() noexcept
		{
			return Vector2(VectorAPI::Set(1.0f, 0.0f, 0.0f, 0.0f));
		}

		/*
		* @brief Returns a unit vector pointing in the Y direction.
		*
		* The resulting vector has components (0, 1).
		*
		* @return A unit vector with only the Y component set to 1.
		*/
		[[nodiscard]] NU_FORCEINLINE static Vector2 UnitY() noexcept
		{
			return Vector2(VectorAPI::Set(0.0f, 1.0f, 0.0f, 0.0f));
		}

		/*
		* @brief Provides read-only access to vector components by index.
		*
		* @param index Index of the component (0 = x, 1 = y).
		* @return The value of the component at the specified index.
		* @throws Assertion failure if index is out of bounds.
		*/
		[[nodiscard]] NU_FORCEINLINE float operator[](int index) const
		{
			assert(index >= 0 && index < 2);
			return m_components[index];
		}

		/*
		* @brief Provides read-write access to vector components by index.
		*
		* @param index Index of the component (0 = x, 1 = y).
		* @return Reference to the component at the specified index.
		* @throws Assertion failure if index is out of bounds.
		*/
		NU_FORCEINLINE float& operator[](int index)
		{
			assert(index >= 0 && index < 2);
			return m_components[index];
		}

		/*
		* @brief Copy assignment operator.
		*
		* @param other The vector to copy from.
		* @return Reference to this vector.
		*/
		NU_FORCEINLINE Vector2& operator=(const Vector2& other) noexcept
		{
			if (this != &other)
			{
				m_data = other.m_data;
			}
			return *this;
		}

		/*
		* @brief Move assignment operator.
		*
		* @param other The vector to move from.
		* @return Reference to this vector.
		*/
		NU_FORCEINLINE Vector2& operator=(Vector2&& other) noexcept
		{
			if (this != &other)
			{
				m_data = std::move(other.m_data);
			}
			return *this;
		}

		/*
		* @brief Calculates the length (magnitude) of the vector.
		*
		* @return Length of the vector.
		*/
		[[nodiscard]] NU_FORCEINLINE float Length() const noexcept
		{
			VectorAPI::NuVec4 mul = VectorAPI::Mul(m_data, m_data);
			return VectorAPI::SqrtScalar(VectorAPI::HorizontalAdd2(mul));
		}

		/*
		* @brief Calculates the squared length of the vector.
		*
		* @return Squared length.
		*/
		[[nodiscard]] NU_FORCEINLINE float LengthSquared() const noexcept
		{
			VectorAPI::NuVec4 mul = VectorAPI::Mul(m_data, m_data);
			return VectorAPI::HorizontalAdd2(mul);
		}

		/*
		* @brief Computes the dot product with another vector.
		*
		* @param other The vector to compute the dot product with.
		* @return Dot product result.
		*/
		[[nodiscard]] NU_FORCEINLINE float Dot(Vector2 other) const noexcept
		{
			VectorAPI::NuVec4 mul = VectorAPI::Mul(m_data, other.m_data);
			float dot = VectorAPI::HorizontalAdd2(mul);
			return dot;
		}

		/*
		* @brief Calculates the distance to another vector.
		*
		* @param other The vector to calculate the distance to.
		* @return Distance between the vectors.
		*/
		[[nodiscard]] NU_FORCEINLINE float Distance(Vector2 other) const noexcept
		{
			return (*this - other).Length();
		}

		/*
		* @brief Provides read-only access to the raw float array of the vector.
		*
		* @return Pointer to the float array.
		*/
		[[nodiscard]] NU_FORCEINLINE const float* Data() const noexcept
		{
			return m_components;
		}

		/*
		* @brief Provides write access to the raw float array of the vector.
		*
		* @return Pointer to the float array.
		*/
		[[nodiscard]] NU_FORCEINLINE float* Data() noexcept
		{
			return m_components;
		}

		/*
		* @brief Returns the underlying SIMD representation.
		*
		* @return Reference to the SIMD vector.
		*/
		[[nodiscard]] NU_FORCEINLINE const VectorAPI::NuVec4& SimdData() const noexcept
		{
			return m_data;
		}

		/*
		* @brief Normalizes the vector.
		*
		* @return Normalized vector.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 Normalize() const noexcept
		{
			return Vector2(VectorAPI::Normalize2(m_data));
		}

		/*
		* @brief Converts the vector to a human-readable string.
		*
		* @return String representation of the vector.
		*/
		[[nodiscard]] std::string ToString() const
		{
			std::ostringstream oss;
			oss << "(" << X() << ", " << Y() << ")";
			return oss.str();
		}

		/*
		* @brief Writes the vector to the output stream.
		*
		* @param os Output stream.
		* @param vec Vector3 to write.
		* @return Reference to the output stream.
		*/
		friend std::ostream& operator<<(std::ostream& os, const Vector2& vec)
		{
			os << vec.ToString();
			return os;
		}

	private:
		/*
		* @brief Storage for vector components.
		*
		* The vector can be accessed either as a SIMD register (m_data) for optimized operations
		* or as an array of floats (m_components) for individual component access.
		* The last 2 components are unused but maintained for SIMD alignment.
		*/
		union
		{
			VectorAPI::NuVec4 m_data; // SIMD-vector
			float m_components[4]; // float-array
		};
	};
}