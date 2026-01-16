// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Algebra/Color/ColorAPI.hpp>
#include <NuMath/Algebra/Vector/Vector4.hpp>
#include <NuMath/Core/Common.hpp>
#include <cmath>
#include <string>
#include <format>
#include <ostream>

namespace NuMath
{
	/**
	 * @brief RGBA Color class backed by SIMD operations.
	 *
	 * @note COLOR SPACE POLICY: LINEAR WORKFLOW
	 * 1. Internal storage is ALWAYS Linear RGB.
	 * 2. Math operations (+, *, Lerp) are physically correct in linear space.
	 * 3. Use Color::FromSRGB() or Color::FromBytes() when loading data (Textures/UI).
	 * 4. Use Color::ToRgba32() to convert back to sRGB for rendering/saving.
	 */
	class alignas(16) Color
	{
	public:
		/**
		 * @brief Default constructor (Transparent Black).
		 */
		NU_FORCEINLINE Color() noexcept
			: m_data(ColorAPI::Colors::Transparent())
		{
		}

		/**
		 * @brief Linear component constructor.
		 *
		 * Assumes inputs are already in Linear RGB space (usually 0.0 to 1.0).
		 *
		 * @param r Red component (linear)
		 * @param g Green component (linear)
		 * @param b Blue component (linear)
		 * @param a Alpha component (linear)
		 */
		NU_FORCEINLINE Color(float r, float g, float b, float a = 1.0f) noexcept
			: m_data(ColorAPI::Set(r, g, b, a))
		{
			NU_MATH_ASSERT(!std::isnan(r) && !std::isnan(g) && !std::isnan(b) && !std::isnan(a),
				"Color components cannot be NaN!");
		}

		/**
		 * @brief Constructs from Vector4.
		 *
		 * @param vec Vector4 containing RGBA components
		 */
		explicit NU_FORCEINLINE Color(const Vector4& vec) noexcept
			: m_data(vec.SimdData())
		{
		}

		/**
		 * @brief Constructs from raw SIMD data.
		 *
		 * @param simdData SIMD vector containing color data
		 */
		explicit NU_FORCEINLINE Color(ColorAPI::NuColor simdData) noexcept
			: m_data(simdData)
		{
		}

		/**
		 * @brief Copy constructor.
		 */
		NU_FORCEINLINE Color(const Color&) noexcept = default;

		/**
		 * @brief Move constructor.
		 */
		NU_FORCEINLINE Color(Color&&) noexcept = default;

		/**
		 * @brief Copy assignment.
		 */
		NU_FORCEINLINE Color& operator=(const Color&) noexcept = default;

		/**
		 * @brief Move assignment.
		 */
		NU_FORCEINLINE Color& operator=(Color&&) noexcept = default;

		/**
		 * @brief Destructor.
		 */
		~Color() = default;

		// =============================================
		// Factory methods
		// =============================================

		/**
		 * @brief Creates a Color from sRGB values (e.g. from Photoshop, Web, Textures).
		 *
		 * Converts sRGB -> Linear using Gamma 2.2 approximation.
		 *
		 * @param r Red in sRGB space [0.0, 1.0]
		 * @param g Green in sRGB space [0.0, 1.0]
		 * @param b Blue in sRGB space [0.0, 1.0]
		 * @param a Alpha (linear) [0.0, 1.0]
		 *
		 * @return Color in linear RGB space
		 */
		[[nodiscard]] static NU_FORCEINLINE Color FromSRGB(float r, float g, float b, float a = 1.0f) noexcept
		{
			return Color(ColorAPI::FromSRGB(r, g, b, a));
		}

		/**
		 * @brief Creates a Color from 8-bit values (0-255, assumed sRGB).
		 *
		 * Uses efficient lookup table for conversion.
		 *
		 * @param r Red [0, 255]
		 * @param g Green [0, 255]
		 * @param b Blue [0, 255]
		 * @param a Alpha [0, 255]
		 *
		 * @return Color in linear RGB space
		 */
		[[nodiscard]] static NU_FORCEINLINE Color FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
		{
			return Color(ColorAPI::FromBytes(r, g, b, a));
		}

		/**
		 * @brief Creates a Color from packed 32-bit RGBA value.
		 *
		 * Format: 0xAABBGGRR (Little Endian)
		 * Assumes input is in sRGB space.
		 *
		 * @param rgba Packed 32-bit color value
		 *
		 * @return Color in linear RGB space
		 */
		[[nodiscard]] static NU_FORCEINLINE Color FromRGBA32(uint32_t rgba) noexcept
		{
			uint8_t r = static_cast<uint8_t>(rgba & 0xFF);
			uint8_t g = static_cast<uint8_t>((rgba >> 8) & 0xFF);
			uint8_t b = static_cast<uint8_t>((rgba >> 16) & 0xFF);
			uint8_t a = static_cast<uint8_t>((rgba >> 24) & 0xFF);
			return FromBytes(r, g, b, a);
		}

		// =============================================
		// Arithmetic operators
		// =============================================

		/**
		 * @brief Component-wise addition.
		 */
		[[nodiscard]] NU_FORCEINLINE Color operator+(const Color& other) const noexcept
		{
			return Color(ColorAPI::Add(m_data, other.m_data));
		}

		/**
		 * @brief Component-wise subtraction.
		 */
		[[nodiscard]] NU_FORCEINLINE Color operator-(const Color& other) const noexcept
		{
			return Color(ColorAPI::Sub(m_data, other.m_data));
		}

		/**
		 * @brief Component-wise multiplication (modulate).
		 */
		[[nodiscard]] NU_FORCEINLINE Color operator*(const Color& other) const noexcept
		{
			return Color(ColorAPI::Mul(m_data, other.m_data));
		}

		/**
		 * @brief Scalar multiplication.
		 */
		[[nodiscard]] NU_FORCEINLINE Color operator*(float scalar) const noexcept
		{
			return Color(ColorAPI::Mul(m_data, ColorAPI::SetAll(scalar)));
		}

		/**
		 * @brief Scalar multiplication (commutative).
		 */
		friend [[nodiscard]] NU_FORCEINLINE Color operator*(float scalar, const Color& color) noexcept
		{
			return color * scalar;
		}

		/**
		 * @brief Component-wise addition assignment.
		 */
		NU_FORCEINLINE Color& operator+=(const Color& other) noexcept
		{
			m_data = ColorAPI::Add(m_data, other.m_data);
			return *this;
		}

		/**
		 * @brief Component-wise subtraction assignment.
		 */
		NU_FORCEINLINE Color& operator-=(const Color& other) noexcept
		{
			m_data = ColorAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/**
		 * @brief Component-wise multiplication assignment.
		 */
		NU_FORCEINLINE Color& operator*=(const Color& other) noexcept
		{
			m_data = ColorAPI::Mul(m_data, other.m_data);
			return *this;
		}

		/**
		 * @brief Scalar multiplication assignment.
		 */
		NU_FORCEINLINE Color& operator*=(float scalar) noexcept
		{
			m_data = ColorAPI::Mul(m_data, ColorAPI::SetAll(scalar));
			return *this;
		}

		// =============================================
		// Comparison operators
		// =============================================

		/**
		 * @brief Exact equality comparison.
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator==(const Color& other) const noexcept
		{
			return ColorAPI::Equal(m_data, other.m_data);
		}

		/**
		 * @brief Inequality comparison.
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator!=(const Color& other) const noexcept
		{
			return !(*this == other);
		}

		/**
		 * @brief Approximate equality with epsilon tolerance.
		 */
		[[nodiscard]] NU_FORCEINLINE bool NearEqual(const Color& other, float epsilon = 0.0001f) const noexcept
		{
			return ColorAPI::NearEqual(m_data, other.m_data, epsilon);
		}

		// =============================================
		// Component accessors
		// =============================================

		/**
		 * @brief Get red component (linear).
		 */
		[[nodiscard]] NU_FORCEINLINE float R() const noexcept
		{
			return ColorAPI::GetX(m_data);
		}

		/**
		 * @brief Get green component (linear).
		 */
		[[nodiscard]] NU_FORCEINLINE float G() const noexcept
		{
			return ColorAPI::GetY(m_data);
		}

		/**
		 * @brief Get blue component (linear).
		 */
		[[nodiscard]] NU_FORCEINLINE float B() const noexcept
		{
			return ColorAPI::GetZ(m_data);
		}

		/**
		 * @brief Get alpha component.
		 */
		[[nodiscard]] NU_FORCEINLINE float A() const noexcept
		{
			return ColorAPI::GetW(m_data);
		}

		/**
		 * @brief Set red component (linear).
		 */
		NU_FORCEINLINE void SetR(float r) noexcept
		{
			NU_MATH_ASSERT(!std::isnan(r), "R cannot be NaN");
			m_data = VectorAPI::SetX(m_data, r);
		}

		/**
		 * @brief Set green component (linear).
		 */
		NU_FORCEINLINE void SetG(float g) noexcept
		{
			NU_MATH_ASSERT(!std::isnan(g), "G cannot be NaN");
			m_data = VectorAPI::SetY(m_data, g);
		}

		/**
		 * @brief Set blue component (linear).
		 */
		NU_FORCEINLINE void SetB(float b) noexcept
		{
			NU_MATH_ASSERT(!std::isnan(b), "B cannot be NaN");
			m_data = VectorAPI::SetZ(m_data, b);
		}

		/**
		 * @brief Set alpha component.
		 */
		NU_FORCEINLINE void SetA(float a) noexcept
		{
			NU_MATH_ASSERT(!std::isnan(a), "A cannot be NaN");
			m_data = VectorAPI::SetW(m_data, a);
		}

		// =============================================
		// Color operations
		// =============================================

		/**
		 * @brief Linear interpolation between two colors.
		 *
		 * @param a Start color (t = 0)
		 * @param b End color (t = 1)
		 * @param t Interpolation parameter [0, 1]
		 *
		 * @return Interpolated color
		 */
		[[nodiscard]] static NU_FORCEINLINE Color Lerp(const Color& a, const Color& b, float t) noexcept
		{
			return Color(ColorAPI::Lerp(a.m_data, b.m_data, t));
		}

		/**
		 * @brief Clamps all components to [0.0, 1.0] range.
		 *
		 * @return Saturated color
		 */
		[[nodiscard]] NU_FORCEINLINE Color Saturate() const noexcept
		{
			return Color(ColorAPI::Saturate(m_data));
		}

		/**
		 * @brief Multiplies RGB by alpha (premultiplied alpha).
		 *
		 * Useful for certain blending modes.
		 *
		 * @return Premultiplied color
		 */
		[[nodiscard]] NU_FORCEINLINE Color PremultiplyAlpha() const noexcept
		{
			return Color(ColorAPI::PremultiplyAlpha(m_data));
		}

		/**
		 * @brief Converts to grayscale using perceptual luminance.
		 *
		 * Uses Rec. 709 coefficients.
		 *
		 * @return Grayscale color with preserved alpha
		 */
		[[nodiscard]] NU_FORCEINLINE Color ToGrayscale() const noexcept
		{
			return Color(ColorAPI::ToGrayscale(m_data));
		}

		/**
		 * @brief Inverts RGB components, preserves alpha.
		 *
		 * @return Inverted color
		 */
		[[nodiscard]] NU_FORCEINLINE Color Invert() const noexcept
		{
			return Color(ColorAPI::Invert(m_data));
		}

		/**
		 * @brief Blends this color over a background color using alpha.
		 *
		 * @param background Background color
		 *
		 * @return Blended result
		 */
		[[nodiscard]] NU_FORCEINLINE Color BlendOver(const Color& background) const noexcept
		{
			return Color(ColorAPI::AlphaBlend(m_data, background.m_data));
		}

		// =============================================
		// Conversion methods
		// =============================================

		/**
		 * @brief Converts to Vector4.
		 *
		 * @return Vector4 representation
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 ToVector4() const noexcept
		{
			return Vector4(m_data);
		}

		/**
		 * @brief Converts to packed 32-bit RGBA integer (sRGB space).
		 *
		 * Applies gamma correction (Linear -> sRGB).
		 * Format: 0xAABBGGRR (Little Endian)
		 *
		 * @return Packed 32-bit RGBA value
		 */
		[[nodiscard]] NU_FORCEINLINE uint32_t ToRGBA32() const noexcept
		{
			return ColorAPI::ToRGBA32(m_data);
		}

		/**
		 * @brief Returns pointer to raw float data.
		 *
		 * @return Pointer to RGBA components
		 */
		[[nodiscard]] NU_FORCEINLINE const float* Data() const noexcept
		{
			return reinterpret_cast<const float*>(&m_data);
		}

		/**
		 * @brief Returns raw SIMD data.
		 *
		 * @return SIMD color representation
		 */
		[[nodiscard]] NU_FORCEINLINE ColorAPI::NuColor SimdData() const noexcept
		{
			return m_data;
		}

		/**
		 * @brief Converts color to string representation.
		 *
		 * @return String in format "Color(r, g, b, a)"
		 */
		[[nodiscard]] NU_FORCEINLINE std::string ToString() const
		{
			return std::format("Color({:.3f}, {:.3f}, {:.3f}, {:.3f})", R(), G(), B(), A());
		}

		/**
		 * @brief Stream output operator.
		 */
		friend std::ostream& operator<<(std::ostream& os, const Color& color)
		{
			os << color.ToString();
			return os;
		}

	private:
		ColorAPI::NuColor m_data;
	};

	// =============================================
	// Color constants
	// =============================================

	namespace Colors
	{
		/**
		 * @brief Linear RGB color constants.
		 *
		 * These colors are stored directly in linear space.
		 * Use for physically correct lighting calculations.
		 */
		namespace Linear
		{
			inline const Color Transparent(0.0f, 0.0f, 0.0f, 0.0f);
			inline const Color Black(0.0f, 0.0f, 0.0f, 1.0f);
			inline const Color White(1.0f, 1.0f, 1.0f, 1.0f);
			inline const Color Red(1.0f, 0.0f, 0.0f, 1.0f);
			inline const Color Green(0.0f, 1.0f, 0.0f, 1.0f);
			inline const Color Blue(0.0f, 0.0f, 1.0f, 1.0f);

		} // namespace Linear

		/**
		 * @brief sRGB color constants.
		 *
		 * These colors match standard sRGB values (like CSS/HTML colors).
		 * Converted to linear space internally.
		 */
		namespace SRGB
		{
			inline const Color Transparent = Color::FromSRGB(0.0f, 0.0f, 0.0f, 0.0f);
			inline const Color Black = Color::FromSRGB(0.0f, 0.0f, 0.0f, 1.0f);
			inline const Color White = Color::FromSRGB(1.0f, 1.0f, 1.0f, 1.0f);
			inline const Color Red = Color::FromSRGB(1.0f, 0.0f, 0.0f, 1.0f);
			inline const Color Green = Color::FromSRGB(0.0f, 1.0f, 0.0f, 1.0f);
			inline const Color Blue = Color::FromSRGB(0.0f, 0.0f, 1.0f, 1.0f);
		} // namespace SRGB
	}

} // namespace NuMath