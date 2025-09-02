// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <variant>
#include <optional>
#include <stdexcept>
#include <type_traits>

#include <Core/Types/Types.hpp>

namespace NuEngine::Core
{
	using NuBool = NuEngine::Core::Types::NuBool;
	
	// -----------------------------------
	// Custom exception type for Result
	// -----------------------------------
	class ResultUnwrapError final : public std::runtime_error
	{
	public:
		explicit ResultUnwrapError(const std::string& msg)
			: std::runtime_error(msg)
		{
		}
	};

	/*
	* Result type that can hold either a value (T) or an error (E).
	* Used for exception-free error handling.
	*/
	template <typename T, typename E>
	class Result
	{
		static_assert(!std::is_void_v<T>,
			"Use Result<void, E> specialization for void results");
	public:
		// Constructors
		Result(const T& value) : m_data(value) {}
		Result(T&& value) : m_data(std::move(value)) {}
		Result(const E& error) : m_data(error) {}
		Result(E&& error) : m_data(std::move(error)) {}

		// Check if result contains a value
		constexpr NuBool IsOk() const noexcept { return std::holds_alternative<T>(m_data); }

		// Check if result contains an error
		constexpr NuBool IsError() const noexcept { return std::holds_alternative<E>(m_data); }

		// Get the value (throws if contains error)
		T& Unwrap()
		{
			if (IsError())
			{
				throw ResultUnwrapError("Attempted to unwrap error result");
			}
			return std::get<T>(m_data);
		}

		const T& Unwrap() const
		{
			if (IsError())
			{
				throw ResultUnwrapError("Attempted to unwrap error result");
			}
			return std::get<T>(m_data);
		}

		// Get the error (throws if contains value)
		E& UnwrapError()
		{
			if (IsOk())
			{
				throw ResultUnwrapError("Attempted to unwrap value result as error");
			}
			return std::get<E>(m_data);
		}

		const E& UnwrapError() const
		{
			if (IsOk())
			{
				throw ResultUnwrapError("Attempted to unwrap value result as error");
			}
			return std::get<E>(m_data);
		}

		// Safe value access (returns nullptr if error)
		constexpr T* TryUnwrap() noexcept
		{
			return IsOk() ? &std::get<T>(m_data) : nullptr;
		}

		constexpr const T* TryUnwrap() const noexcept
		{
			return IsOk() ? &std::get<T>(m_data) : nullptr;
		}

		// Safe error access (returns nullptr if value)
		constexpr E* TryUnwrapError() noexcept
		{
			return IsError() ? &std::get<E>(m_data) : nullptr;
		}

		constexpr const E* TryUnwrapError() const noexcept
		{
			return IsError() ? &std::get<E>(m_data) : nullptr;
		}

		constexpr const T& ValueOr(const T& defaultValue) const
		{
			return IsOk() ? std::get<T>(m_data) : defaultValue;
		}

		constexpr const E& ErrorOr(const E& defaultError) const
		{
			return IsError() ? std::get<E>(m_data) : defaultError;
		}

	private:
		std::variant<T, E> m_data;
	};

	/*
	* Specialization for void results (when no value is returned, only error)
	*/
	template <typename E>
	class Result<void, E>
	{
	public:
		Result() : m_error(std::nullopt) {}
		Result(const E& error) : m_error(error) {}
		Result(E&& error) : m_error(std::move(error)) {}

		constexpr NuBool IsOk() const noexcept { return !m_error.has_value(); }
		constexpr NuBool IsError() const noexcept { return m_error.has_value(); }

		void Unwrap() const
		{
			if (IsError())
			{
				throw ResultUnwrapError("Attempted to unwrap error result");
			}
		}

		E& UnwrapError()
		{
			if (IsOk())
			{
				throw ResultUnwrapError("Attempted to unwrap success result as error");
			}
			return *m_error;
		}

		const E& UnwrapError() const
		{
			if (IsOk())
			{
				throw ResultUnwrapError("Attempted to unwrap success result as error");
			}
			return *m_error;
		}

		constexpr E* TryUnwrapError() noexcept
		{
			return m_error ? &(*m_error) : nullptr;
		}

		constexpr const E* TryUnwrapError() const noexcept
		{
			return m_error ? &(*m_error) : nullptr;
		}

		constexpr const E& ErrorOr(const E& defaultError) const
		{
			return m_error ? *m_error : defaultError;
		}

	private:
		std::optional<E> m_error;
	};

	// -------------------------
	// Convenience factories
	// -------------------------

	template <typename T, typename E>
	NU_FORCEINLINE Result<T, E> Ok(T value) { return Result<T, E>(std::move(value)); }

	template <typename T, typename E>
	NU_FORCEINLINE Result<T, E> Err(E error) { return Result<T, E>(std::move(error)); }

	template <typename E>
	NU_FORCEINLINE Result<void, E> Ok() { return Result<void, E>(); }

	template <typename E>
	NU_FORCEINLINE Result<void, E> Err(E error) { return Result<void, E>(std::move(error)); }
}