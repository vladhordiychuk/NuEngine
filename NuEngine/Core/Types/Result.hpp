// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <variant>
#include <optional>
#include <type_traits>
#include <utility>
#include <functional>

#include <Core/Types/Types.hpp>

namespace NuEngine::Core
{
    // -----------------------------------
    // Wrappers
    // -----------------------------------
    template <typename T> struct OkWrapper { T value; };
    template <typename E> struct ErrWrapper { E error; };
    template <> struct OkWrapper<void> {};

    // -----------------------------------
    // General factories
    // -----------------------------------
    template <typename T>
    constexpr OkWrapper<std::decay_t<T>> Ok(T&& value) noexcept 
    {
        return { std::forward<T>(value) };
    }

    template <typename E>
    constexpr ErrWrapper<std::decay_t<E>> Err(E&& error) noexcept 
    {
        return { std::forward<E>(error) };
    }

    NU_FORCEINLINE constexpr OkWrapper<void> Ok() noexcept { return {}; }

    // -----------------------------------
    // In-place factories
    // -----------------------------------
    template <typename T, typename... Args>
    constexpr OkWrapper<T> MakeOk(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) 
    {
        return { T(std::forward<Args>(args)...) };
    }

    template <typename E, typename... Args>
    constexpr ErrWrapper<E> MakeErr(Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args...>) 
    {
        return { E(std::forward<Args>(args)...) };
    }

    /**
     * Result<T, E> - Rust-style error handling for C++
     *
     * Current implementation: std::variant<T, E>
     *
     * TODO (Performance optimization):
     * - Replace std::variant with custom union for:
     *   * Faster Debug builds (2-3x improvement)
     *   * Smaller binary size (~30% reduction)
     *   * Faster compile times without PCH (~33% improvement)
     *
     * Current implementation is correct and sufficient for development.
     * Optimize only if profiler shows this as bottleneck!
     */
    template <typename T, typename E>
    class [[nodiscard("Result must be handled or explicitly ignored")]] Result
    {
        static_assert(!std::is_void_v<T>,
            "Use Result<void, E> specialization for void results");
        static_assert(!std::is_reference_v<T> && !std::is_reference_v<E>,
            "Result cannot hold reference types directly");
    public:
        using value_type = T;
        using error_type = E;

        constexpr Result(OkWrapper<T> w) noexcept(std::is_nothrow_move_constructible_v<T>)
            : m_data(std::in_place_index<0>, std::move(w.value))
        {
        }

        constexpr Result(ErrWrapper<E> w) noexcept(std::is_nothrow_move_constructible_v<E>)
            : m_data(std::in_place_index<1>, std::move(w.error))
        {
        }

        constexpr Result(Result&&) noexcept(
            std::is_nothrow_move_constructible_v<std::variant<T, E>>) = default;

        constexpr Result(const Result&) noexcept(
            std::is_nothrow_copy_constructible_v<std::variant<T, E>>) = default;

        template <typename ForeignErr>
        requires (std::is_constructible_v<E, ForeignErr> && !std::is_same_v<E, ForeignErr>)
        constexpr Result(ErrWrapper<ForeignErr> w) noexcept(std::is_nothrow_constructible_v<E, ForeignErr>)
            : m_data(std::in_place_index<1>, static_cast<E>(std::move(w.error)))
        {
        }

        constexpr Result& operator=(Result&&) noexcept(
            std::is_nothrow_move_assignable_v<std::variant<T, E>>) = default;

        constexpr Result& operator=(const Result&) noexcept(
            std::is_nothrow_copy_assignable_v<std::variant<T, E>>) = default;

        ~Result() = default;

        template <typename U> 
        Result(U&&) = delete;

        // -------------------------------------------------
        // Transformations
        // -------------------------------------------------

        template <typename F>
        constexpr auto Map(F&& func) const& -> Result<std::invoke_result_t<F, const T&>, E>
        {
            static_assert(std::is_copy_constructible_v<E>,
                "Map(const&) requires Error to be copy-constructible");

            using U = std::invoke_result_t<F, const T&>;
            if (IsError()) 
            {
                return ErrWrapper<E>{ std::get<1>(m_data) };
            }

            if constexpr (std::is_void_v<U>) 
            {
                std::invoke(std::forward<F>(func), std::get<0>(m_data));
                return OkWrapper<void>{};
            }
            else {
                return OkWrapper<U>{ std::invoke(std::forward<F>(func), std::get<0>(m_data)) };
            }
        }

        template <typename F>
        constexpr auto Map(F&& func) && -> Result<std::invoke_result_t<F, T&&>, E>
        {
            using U = std::invoke_result_t<F, T&&>;
            if (IsError()) 
            {
                return ErrWrapper<E>{ std::move(std::get<1>(m_data)) };
            }

            if constexpr (std::is_void_v<U>) 
            {
                std::invoke(std::forward<F>(func), std::move(std::get<0>(m_data)));
                return OkWrapper<void>{};
            }
            else {
                return OkWrapper<U>{ std::invoke(std::forward<F>(func), std::move(std::get<0>(m_data))) };
            }
        }

        template <typename F>
        constexpr auto MapError(F&& func) const& -> Result<T, std::invoke_result_t<F, const E&>>
        {
            static_assert(std::is_copy_constructible_v<T>,
                "MapError(const&) requires Value to be copy-constructible");

            using U = std::invoke_result_t<F, const E&>;
            if (IsOk()) 
            {
                return OkWrapper<T>{ std::get<0>(m_data) };
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), std::get<1>(m_data)) };
        }

        template <typename F>
        constexpr auto MapError(F&& func) && -> Result<T, std::invoke_result_t<F, E&&>>
        {
            using U = std::invoke_result_t<F, E&&>;
            if (IsOk()) 
            {
                return OkWrapper<T>{ std::move(std::get<0>(m_data)) };
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), std::move(std::get<1>(m_data))) };
        }

        template <typename F>
        constexpr auto AndThen(F&& func) const& -> std::invoke_result_t<F, const T&>
        {
            using FuncResult = std::invoke_result_t<F, const T&>;
            static_assert(std::is_same_v<typename FuncResult::error_type, E>,
                "AndThen function must return Result with the same Error type!");

            if (IsError()) 
            {
                return ErrWrapper<E>{ std::get<1>(m_data) };
            }
            return std::invoke(std::forward<F>(func), std::get<0>(m_data));
        }

        template <typename F>
        constexpr auto AndThen(F&& func) && -> std::invoke_result_t<F, T&&>
        {
            using FuncResult = std::invoke_result_t<F, T&&>;
            static_assert(std::is_same_v<typename FuncResult::error_type, E>,
                "AndThen function must return Result with the same Error type!");
            static_assert(!std::is_same_v<value_type, void>,
                "AndThen on void Result should use void specialization");

            if (IsError()) 
            {
                return ErrWrapper<E>{ std::move(std::get<1>(m_data)) };
            }
            return std::invoke(std::forward<F>(func), std::move(std::get<0>(m_data)));
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc) const&
        {
            using OkResult = std::invoke_result_t<OkFunc, const T&>;
            using ErrResult = std::invoke_result_t<ErrFunc, const E&>;
            static_assert(std::is_same_v<OkResult, ErrResult>,
                "Match handlers must return the same type!");

            if (IsOk()) 
            {
                return std::invoke(std::forward<OkFunc>(okFunc), std::get<0>(m_data));
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), std::get<1>(m_data));
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc)&&
        {
            using OkResult = std::invoke_result_t<OkFunc, T&&>;
            using ErrResult = std::invoke_result_t<ErrFunc, E&&>;
            static_assert(std::is_same_v<OkResult, ErrResult>,
                "Match handlers must return the same type!");

            if (IsOk()) 
            {
                return std::invoke(std::forward<OkFunc>(okFunc), std::move(std::get<0>(m_data)));
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), std::move(std::get<1>(m_data)));
        }

        // -------------------------------------------------
        // Status checks
        // -------------------------------------------------

        [[nodiscard]] constexpr bool IsOk() const noexcept 
        {
            return m_data.index() == 0;
        }

        [[nodiscard]] constexpr bool IsError() const noexcept 
        {
            return m_data.index() == 1;
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept 
        {
            return IsOk();
        }

        // -------------------------------------------------
        // Comparison operators
        // -------------------------------------------------

        constexpr bool operator==(const Result& other) const
            noexcept(noexcept(std::declval<T>() == std::declval<T>()) &&
                noexcept(std::declval<E>() == std::declval<E>()))
            requires std::equality_comparable<T>&& std::equality_comparable<E>
        {
            if (m_data.index() != other.m_data.index()) return false;
            if (IsOk()) return std::get<0>(m_data) == std::get<0>(other.m_data);
            return std::get<1>(m_data) == std::get<1>(other.m_data);
        }

        constexpr bool operator!=(const Result& other) const
            noexcept(noexcept(std::declval<Result>() == other))
            requires std::equality_comparable<T>&& std::equality_comparable<E>
        {
            return !(*this == other);
        }

        template <typename U>
        constexpr bool operator==(const OkWrapper<U>& ok) const
            noexcept(noexcept(std::declval<T>() == ok.value))
            requires std::equality_comparable_with<T, U>
        {
            return IsOk() && std::get<0>(m_data) == ok.value;
        }

        template <typename U>
        constexpr bool operator==(const ErrWrapper<U>& err) const
            noexcept(noexcept(std::declval<E>() == err.error))
            requires std::equality_comparable_with<E, U>
        {
            return IsError() && std::get<1>(m_data) == err.error;
        }

        // -------------------------------------------------
        // Safe accessors
        // -------------------------------------------------

        [[nodiscard]] constexpr const T* operator->() const noexcept 
        {
            NU_ASSERT(IsOk(), "Dereferencing error Result");
            return TryUnwrap();
        }

        [[nodiscard]] constexpr T* operator->() noexcept 
        {
            NU_ASSERT(IsOk(), "Dereferencing error Result");
            return TryUnwrap();
        }

        [[nodiscard]] constexpr const T& operator*() const& 
        {
            return Unwrap();
        }

        [[nodiscard]] constexpr T& operator*()& 
        {
            return Unwrap();
        }

        [[nodiscard]] constexpr T&& operator*()&& 
        {
            return std::move(Unwrap());
        }

        // -------------------------------------------------
        // Unsafe accessors (throw on error)
        // -------------------------------------------------

        [[nodiscard]] constexpr T& Unwrap()&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return std::get<0>(m_data);
        }

        [[nodiscard]] constexpr const T& Unwrap() const&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return std::get<0>(m_data);
        }

        [[nodiscard]] constexpr T&& Unwrap()&&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return std::move(std::get<0>(m_data));
        }

        [[nodiscard]] constexpr E& UnwrapError()&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::get<1>(m_data);
        }

        [[nodiscard]] constexpr const E& UnwrapError() const&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::get<1>(m_data);
        }

        [[nodiscard]] constexpr E&& UnwrapError()&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::move(std::get<1>(m_data));
        }

        // -------------------------------------------------
        // Expect variants (with custom message)
        // -------------------------------------------------

        template <typename Msg>
        [[nodiscard]] constexpr T& Expect(Msg&& msg)&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::get<0>(m_data);
        }

        template <typename Msg>
        [[nodiscard]] constexpr const T& Expect(Msg&& msg) const&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::get<0>(m_data);
        }

        template <typename Msg>
        [[nodiscard]] constexpr T&& Expect(Msg&& msg)&&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::move(std::get<0>(m_data));
        }

        template <typename Msg>
        [[nodiscard]] constexpr E& ExpectError(Msg&& msg)&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::get<1>(m_data);
        }

        template <typename Msg>
        [[nodiscard]] constexpr const E& ExpectError(Msg&& msg) const&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::get<1>(m_data);
        }

        template <typename Msg>
        [[nodiscard]] constexpr E&& ExpectError(Msg&& msg)&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::move(std::get<1>(m_data));
        }

        // -------------------------------------------------
        // Safe accessors (return pointer/nullptr)
        // -------------------------------------------------

        [[nodiscard]] constexpr const T* TryUnwrap() const noexcept 
        {
            return IsOk() ? &std::get<0>(m_data) : nullptr;
        }

        [[nodiscard]] constexpr T* TryUnwrap() noexcept 
        {
            return IsOk() ? &std::get<0>(m_data) : nullptr;
        }

        [[nodiscard]] constexpr const E* TryUnwrapError() const noexcept 
        {
            return IsError() ? &std::get<1>(m_data) : nullptr;
        }

        [[nodiscard]] constexpr E* TryUnwrapError() noexcept 
        {
            return IsError() ? &std::get<1>(m_data) : nullptr;
        }

        // -------------------------------------------------
        // Value/Error with default (move-aware)
        // -------------------------------------------------

        template <typename U = T>
        [[nodiscard]] constexpr T ValueOr(U&& defaultValue) const&
        {
            return IsOk() ? std::get<0>(m_data) : static_cast<T>(std::forward<U>(defaultValue));
        }

        template <typename U = T>
        [[nodiscard]] constexpr T ValueOr(U&& defaultValue)&&
        {
            return IsOk() ? std::move(std::get<0>(m_data)) : static_cast<T>(std::forward<U>(defaultValue));
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError) const&
        {
            return IsError() ? std::get<1>(m_data) : static_cast<E>(std::forward<U>(defaultError));
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError)&&
        {
            return IsError() ? std::move(std::get<1>(m_data)) : static_cast<E>(std::forward<U>(defaultError));
        }

        // -------------------------------------------------
        // Optional conversions
        // -------------------------------------------------

        [[nodiscard]] constexpr std::optional<T> ToOptional() const&
        {
            return IsOk() ? std::make_optional(std::get<0>(m_data)) : std::nullopt;
        }

        [[nodiscard]] constexpr std::optional<T> ToOptional()&&
        {
            return IsOk() ? std::make_optional(std::move(std::get<0>(m_data))) : std::nullopt;
        }

        [[nodiscard]] constexpr std::optional<E> ToErrorOptional() const&
        {
            return IsError() ? std::make_optional(std::get<1>(m_data)) : std::nullopt;
        }

        [[nodiscard]] constexpr std::optional<E> ToErrorOptional()&&
        {
            return IsError() ? std::make_optional(std::move(std::get<1>(m_data))) : std::nullopt;
        }

        // -------------------------------------------------
        // Type conversion with compile-time check
        // -------------------------------------------------

        template <typename U>
        [[nodiscard]] constexpr Result<U, E> Cast() const&
        {
            static_assert(std::is_convertible_v<const T&, U>,
                "Cast() requires convertible types");

            if (IsError()) 
            {
                return ErrWrapper<E>{ std::get<1>(m_data) };
            }
            return OkWrapper<U>{ static_cast<U>(std::get<0>(m_data)) };
        }

        template <typename U>
        [[nodiscard]] constexpr Result<U, E> Cast()&&
        {
            static_assert(std::is_convertible_v<T&&, U>,
                "Cast() requires convertible types");

            if (IsError()) 
            {
                return ErrWrapper<E>{ std::move(std::get<1>(m_data)) };
            }
            return OkWrapper<U>{ static_cast<U>(std::move(std::get<0>(m_data))) };
        }

        constexpr void Ignore() const noexcept {}

    private:
        NU_NO_UNIQUE_ADDRESS std::variant<T, E> m_data;
    };

    /**
     * Specialization for void results
     */
    template <typename E>
    class [[nodiscard]] Result<void, E>
    {
        static_assert(!std::is_reference_v<E>,
            "Result cannot hold reference types directly");
    public:
        using value_type = void;
        using error_type = E;

        // Constructors from wrappers ONLY (consistent with general case)
        constexpr Result() noexcept : m_error(std::nullopt) {}
        constexpr Result(OkWrapper<void>) noexcept : m_error(std::nullopt) {}
        constexpr Result(ErrWrapper<E> w) noexcept(std::is_nothrow_move_constructible_v<E>)
            : m_error(std::move(w.error))
        {
        }

        // Rule of 5
        constexpr Result(Result&&) noexcept = default;
        constexpr Result(const Result&) noexcept = default;
        constexpr Result& operator=(Result&&) noexcept = default;
        constexpr Result& operator=(const Result&) noexcept = default;
        ~Result() = default;

        // Deleted dangerous constructor
        template <typename U = E>
        Result(U&& error) = delete;

        // -------------------------------------------------
        // Transformations
        // -------------------------------------------------

        template <typename F>
        constexpr auto Map(F&& func) const -> Result<std::invoke_result_t<F>, E>
        {
            using U = std::invoke_result_t<F>;
            if (IsError()) 
            {
                return ErrWrapper<E>{ *m_error };
            }

            if constexpr (std::is_void_v<U>) 
            {
                std::invoke(std::forward<F>(func));
                return OkWrapper<void>{};
            }
            else {
                return OkWrapper<U>{ std::invoke(std::forward<F>(func)) };
            }
        }

        template <typename F>
        constexpr auto AndThen(F&& func) const -> std::invoke_result_t<F>
        {
            using FuncResult = std::invoke_result_t<F>;
            static_assert(std::is_same_v<typename FuncResult::error_type, E>,
                "AndThen function must return Result with the same Error type!");

            if (IsError()) 
            {
                return ErrWrapper<E>{ *m_error };
            }
            return std::invoke(std::forward<F>(func));
        }

        template <typename F>
        constexpr auto MapError(F&& func) const& -> Result<void, std::invoke_result_t<F, const E&>>
        {
            using U = std::invoke_result_t<F, const E&>;
            if (IsOk()) 
            {
                return {};
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), *m_error) };
        }

        template <typename F>
        constexpr auto MapError(F&& func) && -> Result<void, std::invoke_result_t<F, E&&>>
        {
            using U = std::invoke_result_t<F, E&&>;
            if (IsOk()) 
            {
                return {};
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), std::move(*m_error)) };
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc) const&
        {
            using OkResult = std::invoke_result_t<OkFunc>;
            using ErrResult = std::invoke_result_t<ErrFunc, const E&>;
            static_assert(std::is_same_v<OkResult, ErrResult>,
                "Match handlers must return the same type!");

            if (IsOk()) 
            {
                return std::invoke(std::forward<OkFunc>(okFunc));
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), *m_error);
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc)&&
        {
            using OkResult = std::invoke_result_t<OkFunc>;
            using ErrResult = std::invoke_result_t<ErrFunc, E&&>;
            static_assert(std::is_same_v<OkResult, ErrResult>,
                "Match handlers must return the same type!");

            if (IsOk()) 
            {
                return std::invoke(std::forward<OkFunc>(okFunc));
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), std::move(*m_error));
        }

        // -------------------------------------------------
        // Status checks
        // -------------------------------------------------

        [[nodiscard]] constexpr bool IsOk() const noexcept
        {
            return !m_error.has_value();
        }

        [[nodiscard]] constexpr bool IsError() const noexcept 
        {
            return m_error.has_value();
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept 
        {
            return IsOk();
        }

        // -------------------------------------------------
        // Comparison operators
        // -------------------------------------------------

        constexpr bool operator==(const Result& other) const noexcept
            requires std::equality_comparable<E>
        {
            if (IsOk() && other.IsOk()) return true;
            if (IsError() && other.IsError()) return *m_error == *other.m_error;
            return false;
        }

        constexpr bool operator!=(const Result& other) const noexcept
            requires std::equality_comparable<E>
        {
            return !(*this == other);
        }

        constexpr bool operator==(const OkWrapper<void>&) const noexcept 
        {
            return IsOk();
        }

        template <typename U>
        constexpr bool operator==(const ErrWrapper<U>& err) const
            noexcept(noexcept(std::declval<E>() == err.error))
            requires std::equality_comparable_with<E, U>
        {
            return IsError() && *m_error == err.error;
        }

        // -------------------------------------------------
        // Unsafe accessors
        // -------------------------------------------------

        constexpr void Unwrap() const
        {
            if (IsError())
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
        }

        [[nodiscard]] constexpr E& UnwrapError()&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return *m_error;
        }

        [[nodiscard]] constexpr const E& UnwrapError() const&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return *m_error;
        }

        [[nodiscard]] constexpr E&& UnwrapError()&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::move(*m_error);
        }

        // -------------------------------------------------
        // Expect variants
        // -------------------------------------------------

        template <typename Msg>
        constexpr void Expect(Msg&& msg) const
        {
            if (IsError())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
        }

        template <typename Msg>
        [[nodiscard]] constexpr E& ExpectError(Msg&& msg)&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return *m_error;
        }

        template <typename Msg>
        [[nodiscard]] constexpr const E& ExpectError(Msg&& msg) const&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return *m_error;
        }

        template <typename Msg>
        [[nodiscard]] constexpr E&& ExpectError(Msg&& msg)&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::move(*m_error);
        }

        // -------------------------------------------------
        // Safe accessors
        // -------------------------------------------------

        [[nodiscard]] constexpr const E* TryUnwrapError() const noexcept 
        {
            return m_error ? &(*m_error) : nullptr;
        }

        [[nodiscard]] constexpr E* TryUnwrapError() noexcept
        {
            return m_error ? &(*m_error) : nullptr;
        }

        // -------------------------------------------------
        // Error with default
        // -------------------------------------------------

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError) const&
        {
            return m_error ? *m_error : static_cast<E>(std::forward<U>(defaultError));
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError)&&
        {
            return m_error ? std::move(*m_error) : static_cast<E>(std::forward<U>(defaultError));
        }

        // -------------------------------------------------
        // Optional conversion
        // -------------------------------------------------

        [[nodiscard]] constexpr std::optional<E> ToErrorOptional() const&
        {
            return m_error;
        }

        [[nodiscard]] constexpr std::optional<E> ToErrorOptional()&&
        {
            return std::move(m_error);
        }

        constexpr void Ignore() const noexcept {}

    private:
        NU_NO_UNIQUE_ADDRESS std::optional<E> m_error;
    };

    // -------------------------------------------------
    // Utility functions
    // -------------------------------------------------

    template <typename T, typename E, typename... Args>
    [[nodiscard]] NU_FORCEINLINE constexpr Result<T, E> MakeResultOk(Args&&... args)
    {
        return OkWrapper<T>{ T(std::forward<Args>(args)...) };
    }

    template <typename T, typename E, typename... Args>
    [[nodiscard]] NU_FORCEINLINE constexpr Result<T, E> MakeResultErr(Args&&... args)
    {
        return ErrWrapper<E>{ E(std::forward<Args>(args)...) };
    }
}

#define NU_UNWRAP(variable_name, expr) \
    auto variable_name##_res = (expr); \
    if (variable_name##_res.IsError()) { \
        return NuEngine::Core::Err( \
            NuEngine::Runtime::EngineError( \
                variable_name##_res.UnwrapError().ToString(), \
                std::source_location::current() \
            ) \
        ); \
    } \
    auto variable_name = std::move(variable_name##_res.Unwrap())

#define NU_CHECK(expr) \
    do { \
        auto _res_unique_ = (expr); \
        if (_res_unique_.IsError()) { \
            return NuEngine::Core::Err( \
                NuEngine::Runtime::EngineError( \
                    _res_unique_.UnwrapError().ToString(), \
                    std::source_location::current() \
                ) \
            ); \
        } \
    } while(0)