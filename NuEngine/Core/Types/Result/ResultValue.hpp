// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <optional>
#include <type_traits>
#include <utility>
#include <functional>
#include <memory>
#include <exception>

#include <Core/Platform/CompilerMacros.hpp>
#include <Core/Diagnostics/Assert.hpp>
#include <Core/Types/Result/ResultFwd.hpp>
#include <Core/Types/Result/ResultWrappers.hpp>
#include <Core/Types/Result/ResultCommon.hpp>
#include <Core/Types/Result/Details/ResultStorage.hpp>

namespace NuEngine::Core
{
    template <typename T, typename E>
    class [[nodiscard("Result must be handled or explicitly ignored")]] Result
    {
        static_assert(!std::is_void_v<T>, "Use Result<void, E> specialization for void results");
        static_assert(!std::is_reference_v<T> && !std::is_reference_v<E>, "Result cannot hold reference types directly");
    public:
        using value_type = T;
        using error_type = E;

        NU_FORCEINLINE constexpr Result(OkWrapper<T>&& w) noexcept(std::is_nothrow_constructible_v<Detail::ResultStorage<T, E>, OkWrapper<T>&&>)
            : m_data(std::move(w))
        {
        }

        NU_FORCEINLINE constexpr Result(ErrWrapper<E>&& w) noexcept(std::is_nothrow_constructible_v<Detail::ResultStorage<T, E>, ErrWrapper<E>&&>)
            : m_data(std::move(w))
        {
        }

        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        NU_FORCEINLINE constexpr explicit Result(std::in_place_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<Detail::ResultStorage<T, E>, std::in_place_t, Args...>)
            : m_data(std::in_place, std::forward<Args>(args)...)
        {
        }

        template <typename... Args>
            requires std::is_constructible_v<E, Args...>
        NU_FORCEINLINE constexpr explicit Result(InPlaceErr_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<Detail::ResultStorage<T, E>, InPlaceErr_t, Args...>)
            : m_data(InPlaceErr, std::forward<Args>(args)...)
        {
        }

        template <typename ForeignErr>
            requires (std::is_constructible_v<E, ForeignErr> && !std::is_same_v<E, ForeignErr>)
        constexpr Result(ErrWrapper<ForeignErr>&& w) noexcept(std::is_nothrow_constructible_v<E, ForeignErr>)
            : m_data(ErrWrapper<E>{ static_cast<E>(std::move(w.error)) })
        {
        }

        constexpr Result(const Result&) = default;
        constexpr Result(Result&&) = default;
        constexpr Result& operator=(const Result&) = default;
        constexpr Result& operator=(Result&&) = default;
        constexpr ~Result() = default;

        template <typename U>
            requires (!std::is_same_v<std::decay_t<U>, Result> &&
        !std::is_same_v<std::decay_t<U>, std::in_place_t> &&
            !std::is_same_v<std::decay_t<U>, InPlaceErr_t>)
            Result(U&&) = delete;

        template <typename F>
        constexpr auto Map(F&& func) const& noexcept(std::is_nothrow_invocable_v<F, const T&>&& std::is_nothrow_copy_constructible_v<E>)
            -> Result<std::invoke_result_t<F, const T&>, E>
        {
            static_assert(std::is_copy_constructible_v<E>, "Map(const&) requires Error to be copy-constructible");

            using U = std::invoke_result_t<F, const T&>;
            if (IsError())
            {
                return ErrWrapper<E>{ m_data.Err_() };
            }

            if constexpr (std::is_void_v<U>)
            {
                std::invoke(std::forward<F>(func), m_data.Ok_());
                return OkWrapper<void>{};
            }
            else
            {
                return OkWrapper<U>{ std::invoke(std::forward<F>(func), m_data.Ok_()) };
            }
        }

        template <typename F>
        constexpr auto Map(F&& func) && noexcept(std::is_nothrow_invocable_v<F, T&&>&& std::is_nothrow_move_constructible_v<E>)
            -> Result<std::invoke_result_t<F, T&&>, E>
        {
            using U = std::invoke_result_t<F, T&&>;
            if (IsError())
            {
                return ErrWrapper<E>{ std::move(m_data).Err_() };
            }

            if constexpr (std::is_void_v<U>)
            {
                std::invoke(std::forward<F>(func), std::move(m_data).Ok_());
                return OkWrapper<void>{};
            }
            else
            {
                return OkWrapper<U>{ std::invoke(std::forward<F>(func), std::move(m_data).Ok_()) };
            }
        }

        template <typename F>
        constexpr auto MapError(F&& func) const& noexcept(std::is_nothrow_invocable_v<F, const E&>&& std::is_nothrow_copy_constructible_v<T>)
            -> Result<T, std::invoke_result_t<F, const E&>>
        {
            static_assert(std::is_copy_constructible_v<T>, "MapError(const&) requires Value to be copy-constructible");

            using U = std::invoke_result_t<F, const E&>;
            if (IsOk())
            {
                return OkWrapper<T>{ m_data.Ok_() };
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), m_data.Err_()) };
        }

        template <typename F>
        constexpr auto MapError(F&& func) && noexcept(std::is_nothrow_invocable_v<F, E&&>&& std::is_nothrow_move_constructible_v<T>)
            -> Result<T, std::invoke_result_t<F, E&&>>
        {
            using U = std::invoke_result_t<F, E&&>;
            if (IsOk())
            {
                return OkWrapper<T>{ std::move(m_data).Ok_() };
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), std::move(m_data).Err_()) };
        }

        template <typename F>
        constexpr auto AndThen(F&& func) const& noexcept(std::is_nothrow_invocable_v<F, const T&>&& std::is_nothrow_copy_constructible_v<E>)
            -> std::invoke_result_t<F, const T&>
        {
            using FuncResult = std::invoke_result_t<F, const T&>;
            static_assert(Detail::SameErrorTypeAs<FuncResult, E>, "AndThen function must return Result with the same Error type!");

            if (IsError())
            {
                return ErrWrapper<E>{ m_data.Err_() };
            }
            return std::invoke(std::forward<F>(func), m_data.Ok_());
        }

        template <typename F>
        constexpr auto AndThen(F&& func) && noexcept(std::is_nothrow_invocable_v<F, T&&>&& std::is_nothrow_move_constructible_v<E>)
            -> std::invoke_result_t<F, T&&>
        {
            using FuncResult = std::invoke_result_t<F, T&&>;
            static_assert(Detail::SameErrorTypeAs<FuncResult, E>, "AndThen function must return Result with the same Error type!");
            static_assert(!std::is_same_v<value_type, void>, "AndThen on void Result should use void specialization");

            if (IsError())
            {
                return ErrWrapper<E>{ std::move(m_data).Err_() };
            }
            return std::invoke(std::forward<F>(func), std::move(m_data).Ok_());
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc) const&
            noexcept(std::is_nothrow_invocable_v<OkFunc, const T&>&& std::is_nothrow_invocable_v<ErrFunc, const E&>)
        {
            using OkResult = std::invoke_result_t<OkFunc, const T&>;
            using ErrResult = std::invoke_result_t<ErrFunc, const E&>;
            static_assert(std::is_same_v<OkResult, ErrResult>, "Match handlers must return the same type!");

            if (IsOk())
            {
                return std::invoke(std::forward<OkFunc>(okFunc), m_data.Ok_());
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), m_data.Err_());
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc) &&
            noexcept(std::is_nothrow_invocable_v<OkFunc, T&&>&& std::is_nothrow_invocable_v<ErrFunc, E&&>)
        {
            using OkResult = std::invoke_result_t<OkFunc, T&&>;
            using ErrResult = std::invoke_result_t<ErrFunc, E&&>;
            static_assert(std::is_same_v<OkResult, ErrResult>, "Match handlers must return the same type!");

            if (IsOk())
            {
                return std::invoke(std::forward<OkFunc>(okFunc), std::move(m_data).Ok_());
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), std::move(m_data).Err_());
        }

        [[nodiscard]] NU_FORCEINLINE constexpr bool IsOk() const noexcept { return m_data.IsOk(); }
        [[nodiscard]] NU_FORCEINLINE constexpr bool IsError() const noexcept { return !m_data.IsOk(); }
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return IsOk(); }

        constexpr bool operator==(const Result& other) const
            noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()) &&
                noexcept(std::declval<const E&>() == std::declval<const E&>()))
            requires std::equality_comparable<T>&& std::equality_comparable<E>
        {
            if (IsOk() != other.IsOk())
            {
                return false;
            }

            if (IsOk())
            {
                return m_data.Ok_() == other.m_data.Ok_();
            }
            return m_data.Err_() == other.m_data.Err_();
        }

        constexpr bool operator!=(const Result& other) const
            noexcept(noexcept(std::declval<Result>() == other))
            requires std::equality_comparable<T>&& std::equality_comparable<E>
        {
            return !(*this == other);
        }

        template <typename U>
        constexpr bool operator==(const OkWrapper<U>& ok) const
            noexcept(noexcept(std::declval<const T&>() == ok.value))
            requires std::equality_comparable_with<T, U>
        {
            return IsOk() && m_data.Ok_() == ok.value;
        }

        template <typename U>
        constexpr bool operator==(const ErrWrapper<U>& err) const
            noexcept(noexcept(std::declval<const E&>() == err.error))
            requires std::equality_comparable_with<E, U>
        {
            return IsError() && m_data.Err_() == err.error;
        }

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

        [[nodiscard]] constexpr const T& operator*() const& { return Unwrap(); }
        [[nodiscard]] constexpr const T&& operator*() const&& { return std::move(Unwrap()); }
        [[nodiscard]] constexpr T& operator*()& { return Unwrap(); }
        [[nodiscard]] constexpr T&& operator*()&& { return std::move(*this).Unwrap(); }

        [[nodiscard]] NU_FORCEINLINE constexpr T& Unwrap()&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return m_data.Ok_();
        }

        [[nodiscard]] constexpr const T& Unwrap() const&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return m_data.Ok_();
        }

        [[nodiscard]] constexpr const T&& Unwrap() const&&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return std::move(m_data.Ok_());
        }

        [[nodiscard]] constexpr T&& Unwrap()&&
        {
            if (IsError()) [[unlikely]]
            {
                NU_ASSERT(false, "Panic! Attempted to Unwrap() an Error result.");
                std::terminate();
            }
            return std::move(m_data).Ok_();
        }

        [[nodiscard]] constexpr E& UnwrapError()&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return m_data.Err_();
        }

        [[nodiscard]] constexpr const E& UnwrapError() const&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return m_data.Err_();
        }

        [[nodiscard]] constexpr const E&& UnwrapError() const&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::move(m_data.Err_());
        }

        [[nodiscard]] constexpr E&& UnwrapError()&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::move(m_data).Err_();
        }

        template <typename Msg>
        [[nodiscard]] constexpr T& Expect(Msg&& msg)&
        {
            if (IsError()) [[unlikely]] 
            { 
                NU_ASSERT(false, msg); std::terminate(); 
            }
            return m_data.Ok_();
        }

        template <typename Msg>
        [[nodiscard]] constexpr const T& Expect(Msg&& msg) const&
        {
            if (IsError()) [[unlikely]] 
            { 
                NU_ASSERT(false, msg); std::terminate(); 
            }
            return m_data.Ok_();
        }

        template <typename Msg>
        [[nodiscard]] constexpr const T&& Expect(Msg&& msg) const&&
        {
            if (IsError()) [[unlikely]] 
            { 
                NU_ASSERT(false, msg); std::terminate(); 
            }
            return std::move(m_data.Ok_());
        }

        template <typename Msg>
        [[nodiscard]] constexpr T&& Expect(Msg&& msg)&&
        {
            if (IsError()) [[unlikely]] 
            { 
                NU_ASSERT(false, msg); std::terminate();
            }
            return std::move(m_data).Ok_();
        }

        template <typename Msg>
        [[nodiscard]] constexpr E& ExpectError(Msg&& msg)&
        {
            if (IsOk()) 
            { 
                NU_ASSERT(false, msg); std::terminate(); 
            }
            return m_data.Err_();
        }

        template <typename Msg>
        [[nodiscard]] constexpr const E& ExpectError(Msg&& msg) const&
        {
            if (IsOk()) 
            { 
                NU_ASSERT(false, msg); std::terminate(); 
            }
            return m_data.Err_();
        }

        template <typename Msg>
        [[nodiscard]] constexpr const E&& ExpectError(Msg&& msg) const&&
        {
            if (IsOk()) 
            { 
                NU_ASSERT(false, msg); std::terminate();
            }
            return std::move(m_data.Err_());
        }

        template <typename Msg>
        [[nodiscard]] constexpr E&& ExpectError(Msg&& msg)&&
        {
            if (IsOk()) 
            { 
                NU_ASSERT(false, msg); std::terminate(); 
            }
            return std::move(m_data).Err_();
        }

        [[nodiscard]] constexpr const T* TryUnwrap() const noexcept 
        { 
            return IsOk() ? std::addressof(m_data.Ok_()) : nullptr; 
        }
        [[nodiscard]] constexpr T* TryUnwrap() noexcept 
        { 
            return IsOk() ? std::addressof(m_data.Ok_()) : nullptr; 
        }
        [[nodiscard]] constexpr const E* TryUnwrapError() const noexcept 
        { 
            return IsError() ? std::addressof(m_data.Err_()) : nullptr;
        }
        [[nodiscard]] constexpr E* TryUnwrapError() noexcept 
        { 
            return IsError() ? std::addressof(m_data.Err_()) : nullptr; 
        }

        template <typename U = T>
        [[nodiscard]] constexpr T ValueOr(U&& defaultValue)&
        {
            return IsOk() ? m_data.Ok_() : static_cast<T>(std::forward<U>(defaultValue));
        }

        template <typename U = T>
        [[nodiscard]] constexpr T ValueOr(U&& defaultValue) const&
        {
            return IsOk() ? m_data.Ok_() : static_cast<T>(std::forward<U>(defaultValue));
        }

        template <typename U = T>
        [[nodiscard]] constexpr T ValueOr(U&& defaultValue)&&
        {
            return IsOk() ? std::move(m_data).Ok_() : static_cast<T>(std::forward<U>(defaultValue));
        }

        template <typename U = T>
        [[nodiscard]] constexpr T ValueOr(U&& defaultValue) const&&
        {
            return IsOk() ? m_data.Ok_() : static_cast<T>(std::forward<U>(defaultValue));
        }

        [[nodiscard]] constexpr const T& ValueOrRef(const T& defaultValue) const& noexcept
        {
            return IsOk() ? m_data.Ok_() : defaultValue;
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError)&
        {
            return IsError() ? m_data.Err_() : static_cast<E>(std::forward<U>(defaultError));
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError) const&
        {
            return IsError() ? m_data.Err_() : static_cast<E>(std::forward<U>(defaultError));
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError)&&
        {
            return IsError() ? std::move(m_data).Err_() : static_cast<E>(std::forward<U>(defaultError));
        }

        template <typename U = E>
        [[nodiscard]] constexpr E ErrorOr(U&& defaultError) const&&
        {
            return IsError() ? m_data.Err_() : static_cast<E>(std::forward<U>(defaultError));
        }

        [[nodiscard]] constexpr const E& ErrorOrRef(const E& defaultError) const& noexcept
        {
            return IsError() ? m_data.Err_() : defaultError;
        }

        [[nodiscard]] constexpr std::optional<T> ToOptional() const&
        {
            return IsOk() ? std::make_optional(m_data.Ok_()) : std::nullopt;
        }

        [[nodiscard]] constexpr std::optional<T> ToOptional()&&
        {
            return IsOk() ? std::make_optional(std::move(m_data).Ok_()) : std::nullopt;
        }

        [[nodiscard]] constexpr std::optional<E> ToErrorOptional() const&
        {
            return IsError() ? std::make_optional(m_data.Err_()) : std::nullopt;
        }

        [[nodiscard]] constexpr std::optional<E> ToErrorOptional()&&
        {
            return IsError() ? std::make_optional(std::move(m_data).Err_()) : std::nullopt;
        }

        template <typename U>
        [[nodiscard]] constexpr Result<U, E> Cast() const&
        {
            static_assert(std::is_convertible_v<const T&, U>, "Cast() requires convertible types");
            if (IsError())
            {
                return ErrWrapper<E>{ m_data.Err_() };
            }
            return OkWrapper<U>{ static_cast<U>(m_data.Ok_()) };
        }

        template <typename U>
        [[nodiscard]] constexpr Result<U, E> Cast()&&
        {
            static_assert(std::is_convertible_v<T&&, U>, "Cast() requires convertible types");
            if (IsError())
            {
                return ErrWrapper<E>{ std::move(m_data).Err_() };
            }
            return OkWrapper<U>{ static_cast<U>(std::move(m_data).Ok_()) };
        }

        constexpr void Ignore() const noexcept {}

    private:
        NU_NO_UNIQUE_ADDRESS Detail::ResultStorage<T, E> m_data;
    };
} // namespace NuEngine::Core
