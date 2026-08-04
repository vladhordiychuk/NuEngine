// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>
#include <array>

#include <Core/Types/ErrorContext.hpp>

namespace NuEngine::Core
{
	/**
	 * @brief Stores a fixed-size stack trace of error contexts.
	 *
	 * Collects the sequence of locations where an error has propagated,
	 * allowing diagnostics without dynamic memory allocation.
	 *
	 * @tparam Depth Maximum number of stack frames stored.
	 */
	template <uint8_t Depth = 8>
	struct ErrorTrace
	{
		/// Stored error contexts.
		std::array<ErrorContext, Depth> Frames{};

		/// Number of valid frames currently stored.
		uint8_t Count = 0;

		/**
		 * @brief Appends an error context to the trace.
		 *
		 * The context is ignored if the trace has already reached
		 * its maximum capacity.
		 *
		 * @param ctx Error context to append.
		 */
		constexpr void Push(ErrorContext ctx) noexcept
		{
			if (!Full())
			{
				Frames[Count++] = ctx;
			}
		}

		/**
		 * @brief Checks whether the trace contains any frames.
		 *
		 * @return True if the trace is empty, otherwise false.
		 */
		[[nodiscard]] constexpr bool Empty() const noexcept
		{
			return Count == 0;
		}

		/**
		 * @brief Checks whether the trace has reached its maximum capacity.
		 *
		 * @return True if no additional frames can be stored, otherwise false.
		 */
		[[nodiscard]] constexpr bool Full() const noexcept
		{
			return Count == Depth;
		}

		/**
		 * @brief Returns the maximum number of frames that can be stored.
		 *
		 * @return The trace capacity.
		 */
		[[nodiscard]] constexpr uint8_t Capacity() const noexcept
		{
			return Depth;
		}

		/**
		 * @brief Returns an iterator to the first stored frame.
		 */
		[[nodiscard]] constexpr auto begin() const noexcept 
		{ 
			return Frames.begin(); 
		}

		/**
		 * @brief Returns an iterator one past the last stored frame.
		 */
		[[nodiscard]] constexpr auto end() const noexcept 
		{ 
			return Frames.begin() + Count; 
		}

		/**
		 * @brief Returns the number of stored error contexts.
		 *
		 * @return The number of valid frames in the trace.
		 */
		[[nodiscard]] constexpr uint8_t size() const noexcept
		{
			return Count;
		}
	};

	/**
	 * @brief Zero-storage specialization of ErrorTrace.
	 *
	 * Used by CompactErrorPolicy to completely eliminate stack trace
	 * storage while preserving the same interface.
	 */
	template <>
	struct ErrorTrace<0>
	{
		/**
		 * @brief Discards the supplied error context.
		 */
		constexpr void Push(ErrorContext) noexcept {}

		/**
		 * @brief Always returns true because no frames are stored.
		 * 
		 * @return true.
		 */
		[[nodiscard]] constexpr bool Empty() const noexcept
		{
			return true;
		}

		/**
		 * @brief Always returns true because no frames can be stored.
		 * 
		 * @return true.
		 */
		[[nodiscard]] constexpr bool Full() const noexcept
		{
			return true;
		}

		/**
		 * @brief Always returns zero because no storage is allocated.
		 *
		 * @return Zero.
		 */
		[[nodiscard]] constexpr uint8_t Capacity() const noexcept
		{
			return 0;
		}

		/**
		 * @brief Returns the beginning of the empty trace.
		 *
		 * Always returns nullptr because no storage is allocated.
		 * 
		 * @return nullptr.
		 */
		[[nodiscard]] constexpr ErrorContext* begin() const noexcept
		{
			return nullptr;
		}

		/**
		 * @brief Returns the end of the empty trace.
		 *
		 * Always returns nullptr because no storage is allocated.
		 * 
		 * @return nullptr.
		 */
		[[nodiscard]] constexpr ErrorContext* end() const noexcept
		{
			return nullptr;
		}

		/**
		 * @brief Always returns zero because no frames are stored.
		 * 
		 * @return Zero.
		 */
		[[nodiscard]] constexpr uint8_t size() const noexcept
		{
			return 0;
		}
	};
} // namespace NuEngine::Core