#pragma once

#include "../../SIMD/SIMDBackend.hpp"

#include <string>
#include <sstream>
#include <initializer_list>

namespace Engine::Math
{
    /*
    * @brief 4D Vector with SIMD support.
    * 
    * This class implements a 4-component vector with support for arithmetic operations, 
    * component access, normalization, length, and distance. It supports SSE optimizations.
    */
    class alignas(16) Vector4
    {
    public:
        /*
        * @brief Creates a zero vector (0.0f, 0.0f, 0.0f, 0.0f).
        */
        Vector4() noexcept = default;

        /*
        * @brief Creates a vector with specified components.
        * @param x Value on X.
        * @param y Value on Y.
        * @param z Value on Z.
        * @param w Value on W.
        */
        Vector4(float x, float y, float z, float w) noexcept;

        /*
        * @brief Initialization with initializer_list.
        * @param Initialization list of 4 elements.
        */
        Vector4(std::initializer_list<float> list);

        /*
        * @brief Constructs a copy of another Vector4.
        * 
        * Initializes this vector by copying all components from the given vector.
        * 
        * @param other The vector to copy.
        */
        Vector4(const Vector4& other) noexcept = default;

        /*
        * @brief Constructs a Vector4 by moving another one.
        * 
        * Transfers the internal data from the given vector to this one.
        * The source vector is left in a valid but unspecified state.
        * 
        * @param other The vector to move from.
        */
        Vector4(Vector4&& other) noexcept = default;

        /*
        * @brief Adding another vector to this one.
        * 
        * Performs component-wise addition: each component of another vector is added to the corresponding component of this vector.
        * 
        * @param other The vector to add.
        * @return A new Vector4 that is the result of the addition.
        */
        [[nodiscard]] Vector4 operator+(const Vector4& other) const noexcept;

        /*
        * @brief Subtracting another vector from this one.
        * 
        * Performs component-wise subtraction: each component of another vector is subtracted to the corresponding component of this vector.
        * 
        * @param other The vector to subtract.
        * @return A new Vector4 that is the result of the subtraction.
        */
        [[nodiscard]] Vector4 operator-(const Vector4& other) const noexcept;

        /*
        * @brief Multiplying this vector by another.
        *
        * Performs component-wise multiplication: each component of the other vector
        * is multiplied by the corresponding component of this vector.
        *
        * @param other The vector to multiply with.
        * @return A new Vector4 that is the result of the multiplication.
        */
        [[nodiscard]] Vector4 operator*(const Vector4& other) const noexcept;

        /*
        * @brief Multiplies the vector by a scalar.
        * 
        * Scales each component of the vector by the given scalar value.
        * 
        * @param scalar The scalar value to multiply by.
        * @return A new Vector4 that is the result of the multiplication.
        */
        [[nodiscard]] Vector4 operator*(float scalar) const noexcept;

        /*
        * @brief Divides the vector by a scalar.
        * 
        * Scales each component of the vector by the reciprocal of the given scalar value.
        * 
        * @param scalar The scalar value to divide by.
        * @return A new Vector4 that is the result of the division.
        */
        [[nodiscard]] Vector4 operator/(float scalar) const noexcept;

        /*
        * @brief Adds another vector to this one.
        * 
        * Performs component-wise addition and updates the current vector.
        * 
        * @param other The vector to add.
        * @return A reference to this vector after the addition.
        */
        Vector4& operator+=(const Vector4& other) noexcept;

        /*
        * @brief Subtracts another vector to this one.
        * 
        * Performs component-wise subtraction and updates the current vector.
        * 
        * @param other The vector to subtract.
        * @return A reference to this vector after the subtraction.
        */
        Vector4& operator-=(const Vector4& other) noexcept;

        /*
        * @brief Multiplies another vector to this one.
        * 
        * Performs component-wise multiplication and updates the current vector.
        * 
        * @param other
        * @return A reference to this vector after the multiplication.
        */
        Vector4& operator*=(const Vector4& other) noexcept;

        /*
        * @brief Multiplies this vector by a scalar value.
        * 
        * Each component (x, y, z, w) will be multiplied by the given scalar.
        * 
        * @param scalar The scalar value to multiply the vector by.
        * @return A reference to this vector after the multiplication.
        */
        Vector4& operator*=(float scalar) noexcept;

        /*
        * @brief Divides this vector by a scalar value.
        * 
        * Each component (x, y, z, w) will be divided by the given scalar.
        * 
        * @param scalar The scalar value to divide the vector by.
        * @return A reference to this vector after the division.
        */
        Vector4& operator/=(float scalar) noexcept;

        /*
        * @brief Returns the value of a specific component by index.
        *
        * Valid indices are:
        * - 0: x
        * - 1: y
        * - 2: z
        * - 3: w
        *
        * @param index The index of the component (0–3).
        * @return The value of the requested component.
        * @throws std::out_of_range if the index is not in [0, 3].
        */
        [[nodiscard]] float GetComponent(int index) const;

        /*
        * @brief Returns the x-component
        */
        [[nodiscard]] float X() const noexcept;

        /*
        * @brief Returns the y-component
        */
        [[nodiscard]] float Y() const noexcept;

        /*
        * @brief Returns the z-component
        */
        [[nodiscard]] float Z() const noexcept;

        /*
        * @brief Returns the w-component
        */
        [[nodiscard]] float W() const noexcept;

        /*
        * @brief Compares two Vector4 instances for equality.
        * 
        * @param other The vector to compare with.
        * @result true if all components are equal, false otherwise.
        */
        [[nodiscard]] bool operator==(const Vector4& other) const noexcept;

        /*
        * @brief Compares two Vector4 instances for inequality.
        * 
        * @param other The vector to compare with.
        * @result true if any component differs, false otherwise.
        */
        [[nodiscard]] bool operator!=(const Vector4& other) const noexcept;

        /*
        * @brief Returns a negated copy of the vector.
        * 
        * Each component of the resulting vector is the negation of the corresponding component in this vector.
        * 
        * @return A negated vector
        */
        [[nodiscard]] Vector4 operator-() const noexcept;

        /*
        * @brief Provides read-only access to vector components by index.
        *
        * @param index Index of the component (0 = x, 1 = y, 2 = z, 3 = w).
        * @return The value of the component at the specified index.
        * @throws std::out_of_range if index is not in [0, 3].
        */
        [[nodiscard]] float operator[](int) const;

        /*
        * @brief Copy assignment operator.
        *
        * @param other The vector to copy from.
        * @return Reference to this vector.
        */
        Vector4& operator=(const Vector4& other) noexcept = default;

        /*
        * @brief Move assignment operator.
        *
        * @param other The vector to move from.
        * @return Reference to this vector.
        */
        Vector4& operator=(Vector4&& other) noexcept = default;

        /*
        * @brief Returns a zero vector.
        * 
        * The resulting vector has components (0, 0, 0, 0).
        * 
        * @return A vector with all components set to 0.
        */
        [[nodiscard]] static Vector4 Zero() noexcept;

        /*
        * @brief Returns a vector with all components set to 1.
        * 
        * The resulting vector has components (1, 1, 1, 1).
        * 
        * @return A vector with all components set to 1.
        */
        [[nodiscard]] static Vector4 One() noexcept;

        /*
        * @brief Returns a unit vector pointing in the X direction.
        * 
        * The resulting vector has components (1, 0, 0, 0).
        * 
        * @return A unit vector with only the X component set to 1.
        */
        [[nodiscard]] static Vector4 UnitX() noexcept;

        /*
        * @brief Returns a unit vector pointing in the Y direction.
        * 
        * The resulting vector has components (0, 1, 0, 0).
        * 
        * @return A unit vector with only the Y component set to 1.
        */
        [[nodiscard]] static Vector4 UnitY() noexcept;

        /*
        * @brief Returns a unit vector pointing in the Z direction.
        * 
        * The resulting vector has components (0, 0, 1, 0).
        * 
        * @return A unit vector with only the Z component set to 1.
        */
        [[nodiscard]] static Vector4 UnitZ() noexcept;

        /*
        * @brief Returns a unit vector pointing in the W direction.
        * 
        * The resulting vector has components (0, 0, 0, 1).
        * 
        * @return A unit vector with only the W component set to 1.
        */
        [[nodiscard]] static Vector4 UnitW() noexcept;

        /*
        * @brief Calculates the length (magnitude) of the vector.
        * 
        * @return Length of the vector.
        */
        [[nodiscard]] float Length() const noexcept;

        /*
        * @brief Computes the dot product with another vector.
        * 
        * @param other The vector to compute the dot product with.
        * @return Dot product result.
        */
        [[nodiscard]] float Dot(const Vector4& other) const noexcept;

        /*
        * @brief Calculates the squared length of the vector.
        * 
        * @return Squared length.
        */
        [[nodiscard]] float LengthSquared() const noexcept;

        /*
        * @brief Calculates the distance to another vector.
        * 
        * @param other The vector to calculate the distance to.
        * @return Distance between the vectors.
        */
        [[nodiscard]] float Distance(const Vector4& other) const noexcept;

        /*
        * @brief Provides read-only access to the raw float array of the vector.
        * 
        * @return Pointer to the float array.
        */
        [[nodiscard]] const float* Data() const noexcept;

        /*
        * @brief Provides write access to the raw float array of the vector.
        * 
        * @return Pointer to the float array.
        */
        [[nodiscard]] float* Data() noexcept;

        /*
        * @brief Returns the underlying SIMD representation.
        * 
        * @return Reference to the SIMD vector.
        */
        [[nodiscard]] const Simd::NuVec4& SimdData() const noexcept;

        /*
        * @brief Normalizes the vector
        * @return Normalized vector
        */
        [[nodiscard]] Vector4 Normalize() const noexcept;

        /*
        * @brief Converts the vector to a human-readable string.
        * 
        * @return String representation of the vector.
        */
        [[nodiscard]] std::string ToString() const;

        /*
        * @brief Writes the vector to the output stream.
        * 
        * @param os Output stream.
        * @param vec Vector to write.
        * @return Reference to the output stream.
        */
        friend std::ostream& operator<<(std::ostream&, const Vector4&);

    private:
        /*
        * @brief SIMD-data that stores vector components
        */
        Simd::NuVec4 m_data;

        /*
        * @brief Constructs a Vector4 from a SIMD vector.
        * 
        * Initializes the Vector4 using the given SIMD vector without performing
        * any additional computations or conversions.
        * 
        * @param simd The SIMD vector (NuVec4) used to initialize this Vector4.
        */
        explicit Vector4(Simd::NuVec4 simd) noexcept : m_data(simd) {}
    };
}