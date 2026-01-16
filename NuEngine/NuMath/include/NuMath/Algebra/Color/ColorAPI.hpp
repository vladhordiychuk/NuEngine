// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Detail/SIMD/SimdBackend.hpp>
#include <NuMath/Algebra/Vector/VectorAPI.hpp>
#include <cmath>

namespace NuMath::ColorAPI
{
	using Impl = Simd::MathBackend;

	// =============================================
	// Type definitions
	// =============================================

	using NuColor = Impl::NuVec4;

	// =============================================
	// Basic operations (reuse from VectorAPI)
	// =============================================

	using VectorAPI::Load;
	using VectorAPI::Store;
	using VectorAPI::Set;
	using VectorAPI::SetAll;
	using VectorAPI::Add;
	using VectorAPI::Sub;
	using VectorAPI::Mul;
	using VectorAPI::Lerp;
	using VectorAPI::Min;
	using VectorAPI::Max;
	using VectorAPI::GetX;
	using VectorAPI::GetY;
	using VectorAPI::GetZ;
	using VectorAPI::GetW;
	using VectorAPI::Equal;
	using VectorAPI::NearEqual;

	// =============================================
	// Color-specific operations
	// =============================================

	/**
	 * @brief Creates a color from sRGB values using gamma 2.2 approximation.
	 *
	 * Converts sRGB color space to linear RGB for physically correct rendering.
	 *
	 * @param r Red component in sRGB space [0.0, 1.0]
	 * @param g Green component in sRGB space [0.0, 1.0]
	 * @param b Blue component in sRGB space [0.0, 1.0]
	 * @param a Alpha component (linear) [0.0, 1.0]
	 *
	 * @return NuColor in linear RGB space
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor FromSRGB(float r, float g, float b, float a = 1.0f) noexcept
	{
		return Set(
			std::pow(r, 2.2f),
			std::pow(g, 2.2f),
			std::pow(b, 2.2f),
			a
		);
	}

	/**
	 * @brief Creates a color from 8-bit values (0-255) in sRGB space.
	 *
	 * Uses lookup table for efficient conversion from sRGB bytes to linear RGB.
	 *
	 * @param r Red component [0, 255]
	 * @param g Green component [0, 255]
	 * @param b Blue component [0, 255]
	 * @param a Alpha component [0, 255]
	 *
	 * @return NuColor in linear RGB space
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
	{
		extern const float SRGBToLinearLUT[256];

		return Set(
			SRGBToLinearLUT[r],
			SRGBToLinearLUT[g],
			SRGBToLinearLUT[b],
			a * (1.0f / 255.0f)
		);
	}

	/**
	 * @brief Clamps color components to [0.0, 1.0] range.
	 *
	 * @param color Input color
	 *
	 * @return Saturated color with all components in valid range
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor Saturate(NuColor color) noexcept
	{
		NuColor zero = VectorAPI::SetZero();
		NuColor one = Set(1.0f, 1.0f, 1.0f, 1.0f);
		return Min(Max(color, zero), one);
	}

	/**
	 * @brief Converts linear RGB color to gamma 2.2 (sRGB approximation).
	 *
	 * @param r Linear red component
	 * @param g Linear green component
	 * @param b Linear blue component
	 * @param a Alpha component (remains linear)
	 *
	 * @return NuColor in sRGB space
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor ToSRGB(float r, float g, float b, float a = 1.0f) noexcept
	{
		return Set(
			std::pow(r, 1.0f / 2.2f),
			std::pow(g, 1.0f / 2.2f),
			std::pow(b, 1.0f / 2.2f),
			a
		);
	}

	/**
	 * @brief Converts linear RGB color to packed 32-bit RGBA (sRGB).
	 *
	 * Format: 0xAABBGGRR (Little Endian)
	 * Applies gamma correction and converts to 8-bit per channel.
	 *
	 * @param color Linear RGB color to convert
	 *
	 * @return 32-bit packed RGBA value in sRGB space
	 */
	[[nodiscard]] NU_FORCEINLINE uint32_t ToRGBA32(NuColor color) noexcept
	{
		NuColor saturated = Saturate(color);

		float r_linear = GetX(saturated);
		float g_linear = GetY(saturated);
		float b_linear = GetZ(saturated);
		float a_linear = GetW(saturated);

		float r_srgb = std::pow(r_linear, 1.0f / 2.2f);
		float g_srgb = std::pow(g_linear, 1.0f / 2.2f);
		float b_srgb = std::pow(b_linear, 1.0f / 2.2f);

		uint32_t r = static_cast<uint32_t>(r_srgb * 255.0f + 0.5f);
		uint32_t g = static_cast<uint32_t>(g_srgb * 255.0f + 0.5f);
		uint32_t b = static_cast<uint32_t>(b_srgb * 255.0f + 0.5f);
		uint32_t a = static_cast<uint32_t>(a_linear * 255.0f + 0.5f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}

	/**
	 * @brief Multiplies RGB components by alpha (premultiplied alpha).
	 *
	 * Useful for alpha blending operations.
	 *
	 * @param color Input color
	 *
	 * @return Color with RGB premultiplied by alpha
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor PremultiplyAlpha(NuColor color) noexcept
	{
		float a = GetW(color);
		NuColor alpha_vec = SetAll(a);

		// Keep alpha unchanged, multiply RGB only
		NuColor rgb_premul = Mul(color, alpha_vec);

		// Restore original alpha in W component
		return VectorAPI::SetW(rgb_premul, a);
	}

	/**
	 * @brief Creates a grayscale color using luminance weights.
	 *
	 * Uses standard Rec. 709 coefficients for perceptual luminance.
	 *
	 * @param color Input color
	 *
	 * @return Grayscale color with preserved alpha
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor ToGrayscale(NuColor color) noexcept
	{
		float r = GetX(color);
		float g = GetY(color);
		float b = GetZ(color);
		float a = GetW(color);

		// Rec. 709 luminance weights
		float luminance = 0.2126f * r + 0.7152f * g + 0.0722f * b;

		return Set(luminance, luminance, luminance, a);
	}

	/**
	 * @brief Inverts RGB components while preserving alpha.
	 *
	 * @param color Input color
	 *
	 * @return Inverted color (1.0 - RGB)
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor Invert(NuColor color) noexcept
	{
		NuColor one = Set(1.0f, 1.0f, 1.0f, 0.0f);
		NuColor inverted = Sub(one, color);

		// Restore original alpha
		return VectorAPI::SetW(inverted, GetW(color));
	}

	/**
	 * @brief Blends two colors using alpha blending.
	 *
	 * Formula: result = src * src.a + dst * (1 - src.a)
	 *
	 * @param src Source color (foreground)
	 * @param dst Destination color (background)
	 *
	 * @return Blended color
	 */
	[[nodiscard]] NU_FORCEINLINE NuColor AlphaBlend(NuColor src, NuColor dst) noexcept
	{
		float src_alpha = GetW(src);
		float inv_alpha = 1.0f - src_alpha;

		NuColor src_scaled = Mul(src, SetAll(src_alpha));
		NuColor dst_scaled = Mul(dst, SetAll(inv_alpha));

		return Add(src_scaled, dst_scaled);
	}

	/**
	 * @brief Creates a predefined color constant.
	 */
	namespace Colors
	{
		[[nodiscard]] NU_FORCEINLINE NuColor Transparent() noexcept { return Set(0.0f, 0.0f, 0.0f, 0.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Black() noexcept { return Set(0.0f, 0.0f, 0.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor White() noexcept { return Set(1.0f, 1.0f, 1.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Red() noexcept { return Set(1.0f, 0.0f, 0.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Green() noexcept { return Set(0.0f, 1.0f, 0.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Blue() noexcept { return Set(0.0f, 0.0f, 1.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Yellow() noexcept { return Set(1.0f, 1.0f, 0.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Cyan() noexcept { return Set(0.0f, 1.0f, 1.0f, 1.0f); }
		[[nodiscard]] NU_FORCEINLINE NuColor Magenta() noexcept { return Set(1.0f, 0.0f, 1.0f, 1.0f); }
	}

} // namespace NuMath::ColorAPI