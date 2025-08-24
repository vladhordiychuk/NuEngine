// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Detail/SIMD/SIMDBackend.hpp>

namespace NuEngine::Math::VectorAPI
{
	// =============================================
	// Type definitions
	// =============================================

	using NuVec4 = Simd::NuVec4;

	// =============================================
	// Setters
	// =============================================

	/*
	* @brief Create a 4-component vector with specified values.
	*
	* Initializes a NuVec4 with the given component values.
	* The w component is optional and defaults to 0.0f.
	*
	* @param x The x-component value.
	* @param y The y-component value.
	* @param z The z-component value.
	* @param w The w-component value.
	*
	* @return New NuVec4 with the specified component values.
	*/
	using Simd::Set;

	/*
	* @brief Create a 4-component vector with identical values.
	*
	* Initializes all components (x, y, z, w) to the same scalar value.
	*
	* @param scalar Value to set for all components.
	*
	* @return New NuVec4 with all components equal to scalar.
	*/
	using Simd::SetAll;

	/*
	* @brief Create a 4-component vector with zero values.
	*
	* Initializes all components (x, y, z, w) to the zero.
	*
	* @return New NuVec4 with all components equal to zero.
	*/
	using Simd::SetZero;

	// =============================================
	// Getters
	// =============================================

	/*
	* @brief Get the X component of 4D vector.
	*
	* Extracts and returns the first component (X) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of X component.
	*/
	using Simd::GetX;

	/*
	* @brief Get the Y component of 4D vector.
	*
	* Extracts and returns the second component (Y) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of Y component.
	*/
	using Simd::GetY;

	/*
	* @brief Get the Z component of 4D vector.
	*
	* Extracts and returns the third component (Z) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of Z component.
	*/
	using Simd::GetZ;

	/*
	* @brief Get the W component of 4D vector.
	*
	* Extracts and returns the fouth component(W) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of W component.
	*/
	using Simd::GetW;

	// =============================================
	// Basic Arithmetic
	// =============================================

	/*
	* @brief Component-wise addition of two 4D vectors.
	*
	* Performs scalar addition.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is a[i] + b[i].
	*/
	using Simd::Add;

	/*
	* @brief Component-wise subtaction of two 4D vectors.
	*
	* Perfoem scalar subtraction.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is a[i] - b[i].
	*/
	using Simd::Sub;

	/*
	* @brief Component-wise multiplication of two 4D vectors.
	*
	* Perform scalar multiplication.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component a[i] * b[i].
	*/
	using Simd::Mul;

	/*
	* @brief Component-wise division of two 4D vectors.
	*
	* Perform scalar division.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component a[i] / b[i].
	*/
	using Simd::Div;

	/*
	* @brief Negates all components of a 4D vector.
	*
	* Computes component-wise negation: [-x, -y, -z, -w].
	*
	* @param a Input vector.
	*
	* @return New NuVec4 with negated components.
	*/
	using Simd::Neg;

	/*
	* @brief Component-wise minimum of two vectors.
	*
	* Computes a new vector where each component is the minimum
	* of corresponding components from input vectors.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is min(a[i], b[i]).
	*/
	using Simd::Min;

	/*
	* @brief Component-wise maximum of two vectors.
	*
	* Computes a new vector where each component is the maximum
	* of corresponding components from input vectors.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is max(a[i], b[i]).
	*/
	using Simd::Max;

	/*
	* @brief Component-wise absolute value.
	*
	* Returns vector with |x|, |y|, |z|, |w| components
	*
	* @param v Input vector.
	*
	* @return New NuVec4 with absolute values.
	*/
	using Simd::Abs;

	/*
	* @brief Exact floating-point vector equality comparison.
	*
	* Performs component-wise == comparison. For approximate equality,
	* consider using a threshold-based comparison instead.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return true if all components are exactly equal.
	* @warning Floating-point imprecision may affect results.
	*/
	using Simd::Equal;

	/*
	* @brief Approximate floating-point vector equality comparison with epsilon.
	*
	* Performs component-wise comparison with given epsilon tolerance.
	*
	* @param a First vector.
	* @param b Second vector.
	* @param epsilon The tolerance value.
	*
	* @return true if all components differ by no more than epsilon.
	*/
	using Simd::NearEqual;

	/*
	* @brief Computes the square root of a scalar floating-point value.
	*
	* Calculates the square root.
	*
	* @param scalar The input value.
	*
	* @return The square root of the input value.
	* @warning Scalar value must be >= 0 for valid result (Input values < 0 will return NaN).
	*/
	using Simd::SqrtScalar;

	// =============================================
	// Special Ops
	// =============================================

	/*
	* @brief Computes the horizontal sum of the 4D vector.
	*
	* Efficiently calculates the sum x + y + z + w of a 4D vector.
	*
	* @param v Input vector.
	*
	* @return Sum of x, y, z and w components.
	*/
	using Simd::HorizontalAdd4;

	/*
	* @brief Computes the horizontal sum of the first three components of a 4D vector.
	*
	* Efficiently calculates the sum x + y + z of a 4D vector (ignores w component).
	*
	* @param v Input vector (const ref).
	*
	* @return Sum of x, y, and z components.
	*/
	using Simd::HorizontalAdd3;

	/*
	* @brief Computes the horizontal sum of the first two components of a 4D vector.
	*
	* Efficiently calculates the sum x + y of a 4D vector (ignores z and w components).
	*
	* @param v Input vector (const ref).
	*
	* @return Sum of x and y components.
	*/
	using Simd::HorizontalAdd2;

	/*
	* @brief NNormalizes first 2 components (x,y) of 4D vector.
	*
	* Computes a unit vector from x/y components while leaving z/w unchanged.
	*
	* @param v Input vector (const ref).
	*
	* @return Normalized vector with original z/w.
	*/
	using Simd::Normalize2;

	/*
	* @brief Normalizes first 3 components (x,y,z) of 4D vector.
	*
	* Computes unit vector from x,y,z components while preserving w.
	*
	* @param v Input vector (const ref).
	*
	* @return Normalized vector with original w component.
	*/
	using Simd::Normalize3;

	/*
	* @brief Normalizes full 4D vector.
	*
	* Computes unit vector from all components (x,y,z,w).
	*
	* @param v Input vector (const ref).
	*
	* @return Fully normalized vector.
	*/
	using Simd::Normalize4;

	// =============================================
	// Extra Math Utilities
	// =============================================

	/*
	* @brief Computes 3D cross product of two vectors (ignores w components).
	*
	* Calculates the cross product for the first three components (x,y,z), setting w component to 0.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return Cross product vector {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0}.
	*/
	using Simd::Cross;

	/*
	* @brief Computes 2D dot product (ignores w components)
	*
	* Calculates the dot product of the first two components (x,y).
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return Dot product (a.x*b.x + a.y*b.y).
	*/
	using Simd::Dot2;

	/*
	* @brief Computes 3D dot product (ignores w components)
	*
	* Calculates the dot product of the first three components (x,y,z).
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return Dot product (a.x*b.x + a.y*b.y + a.z*b.z).
	*/
	using Simd::Dot3;

	/*
	* @brief Computes 4D dot product (ignores w components)
	*
	* Calculates the dot product of the four components (x,y,z,w).
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return Dot product (a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w).
	*/
	using Simd::Dot4;

	/*
	* @brief Computes 2D vector length (ignores z and w components).
	*
	* Calculates the Euclidean length of the first two components (x,y).
	*
	* @param v Input vector.
	* @return Length of vector (sqrt(x² + y²)).
	*/
	using Simd::Length2;

	/*
	* @brief Computes 3D vector length (ignores w component).
	*
	* Calculates the Euclidean length of the first three components (x,y,z).
	*
	* @param v Input vector.
	* @return Length of vector (sqrt(x² + y² + z²)).
	*/
	using Simd::Length3;

	/*
	* @brief Computes 4D vector length.
	*
	* Calculates the Euclidean length of the four components (x,y,z,w).
	*
	* @param v Input vector.
	* @return Length of vector (sqrt(x² + y² + z² + w²)).
	*/
	using Simd::Length4;
}