// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <system_error>
#include <string>
#include <string_view>

#include <Core/Types/ErrorTrace.hpp>
#include <Core/Types/StaticString.hpp>

namespace NuEngine::Core
{
	/**
	 * @brief Placeholder type representing the absence of an error message.
	 *
	 * Used by CompactErrorPolicy to eliminate message storage entirely.
	 * All functions return empty values and are optimized away by the compiler.
	 */
	struct NoMessage final
	{
		constexpr NoMessage() = default;

		/**
		 * @brief Accepts a string for interface compatibility but ignores it.
		 */
		constexpr NoMessage(std::string_view) noexcept {}

		/**
		 * @brief Returns an empty string view.
		 */
		[[nodiscard]] constexpr std::string_view View() const noexcept
		{
			return {};
		}

		/**
		 * @brief Always returns true because no message is stored.
		 */
		[[nodiscard]] constexpr bool Empty() const noexcept
		{
			return true;
		}
	};

	static_assert(std::is_empty_v<NoMessage>);
	static_assert(std::is_trivially_copyable_v<NoMessage>);

	/**
	 * @brief Placeholder type representing the absence of a file path.
	 *
	 * Used when storing the path associated with an error is unnecessary.
	 */
	struct NoPath final
	{
		constexpr NoPath() = default;

		/**
		 * @brief Accepts a path for interface compatibility but ignores it.
		 */
		constexpr NoPath(std::string_view) noexcept {}

		/**
		 * @brief Returns an empty string view.
		 */
		[[nodiscard]] constexpr std::string_view View() const noexcept
		{
			return {};
		}

		/**
		 * @brief Always returns true because no path is stored.
		 */
		[[nodiscard]] constexpr bool Empty() const noexcept
		{
			return true;
		}
	};

	static_assert(std::is_empty_v<NoPath>);
	static_assert(std::is_trivially_copyable_v<NoPath>);

	/**
		 * @brief Placeholder type representing the absence of a system error.
		 *
		 * Behaves similarly to std::error_code but always represents "no error"
		 * and occupies no meaningful storage.
		 */
	struct NoSystemError final
	{
		constexpr NoSystemError() = default;

		/**
		 * @brief Accepts an error code for interface compatibility but discards it.
		 * Прибрано constexpr, оскільки std::error_code не є literal type
		 */
		NoSystemError(const std::error_code&) noexcept {}

		/**
		 * @brief Always returns false because no system error is stored.
		 */
		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return false;
		}

		/**
		 * @brief Returns an empty error message.
		 */
		[[nodiscard]] constexpr std::string message() const noexcept
		{
			return {};
		}

		/**
		 * @brief Always returns zero.
		 */
		[[nodiscard]] constexpr int value() const noexcept
		{
			return 0;
		}
	};

	static_assert(std::is_empty_v<NoSystemError>);
	static_assert(std::is_trivially_copyable_v<NoSystemError>);

	/**
	 * @brief Generic error information policy.
	 *
	 * Combines the storage types used by Error into a single policy.
	 *
	 * @tparam TMessage      Type used to store the error message.
	 * @tparam TPath         Type used to store the file path.
	 * @tparam TTrace        Type used to store the stack trace.
	 * @tparam TSystemError  Type used to store the system error.
	 */
	template <typename TMessage, typename TPath, typename TTrace, typename TSystemError>
	struct BasicErrorPolicy
	{
		using Message = TMessage;
		using Path = TPath;
		using Trace = TTrace;
		using SystemError = TSystemError;
	};

	/**
	 * @brief Minimal error information policy.
	 *
	 * Stores no diagnostic information, including messages, file paths,
	 * system errors, or stack traces.
	 *
	 * Intended for memory-constrained or performance-critical builds.
	 */
	using CompactErrorPolicy = BasicErrorPolicy<NoMessage, NoPath, ErrorTrace<0>, NoSystemError>;

	/**
	 * @brief Balanced error information policy.
	 *
	 * Stores a small fixed-size message and a short stack trace while
	 * avoiding dynamic memory allocation.
	 */
	using MediumErrorPolicy = BasicErrorPolicy<StaticString<64>, NoPath, ErrorTrace<2>, NoSystemError>;

	/**
	 * @brief Full-featured error information policy.
	 *
	 * Stores fixed-size messages, file paths, system error codes,
	 * and an extended stack trace for detailed diagnostics without
	 * dynamic memory allocation.
	 */
	using RichErrorPolicy = BasicErrorPolicy<MessageString, PathString, ErrorTrace<8>, std::error_code>;

} // namespace NuEngine::Core