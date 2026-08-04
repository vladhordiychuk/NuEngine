// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <optional>
#include <type_traits>
#include <utility>
#include <functional>
#include <exception>

#include <Core/Platform/CompilerMacros.hpp>
#include <Core/Diagnostics/Assert.hpp>
#include <Core/Types/Result/ResultFwd.hpp>
#include <Core/Types/Result/ResultWrappers.hpp>
#include <Core/Types/Result/ResultCommon.hpp>

namespace NuEngine::Core
{
    template <typename E>
    class [[nodiscard]] Result<void, E>
    {
        static_assert(!std::is_reference_v<E>, "Result cannot hold reference types directly");
    public:
        using value_type = void;
        using error_type = E;

        constexpr Result() noexcept : m_error(std::nullopt) {}
        constexpr Result(OkWrapper<void>) noexcept : m_error(std::nullopt) {}
        constexpr Result(ErrWrapper<E>&& w) noexcept(std::is_nothrow_move_constructible_v<E>)
            : m_error(std::move(w.error))
        {
        }

        template <typename... Args>
            requires std::is_constructible_v<E, Args...>
        NU_FORCEINLINE constexpr explicit Result(InPlaceErr_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
            : m_error(std::in_place, std::forward<Args>(args)...)
        {
        }

        constexpr Result(Result&&) noexcept = default;
        constexpr Result(const Result&) noexcept = default;
        constexpr Result& operator=(Result&&) noexcept = default;
        constexpr Result& operator=(const Result&) noexcept = default;
        constexpr ~Result() = default;

        template <typename U = E>
            requires (!std::is_same_v<std::decay_t<U>, InPlaceErr_t>)
        Result(U&& error) = delete;

        template <typename F>
        constexpr auto Map(F&& func) const noexcept(std::is_nothrow_invocable_v<F>&& std::is_nothrow_copy_constructible_v<E>)
            -> Result<std::invoke_result_t<F>, E>
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
            else
            {
                return OkWrapper<U>{ std::invoke(std::forward<F>(func)) };
            }
        }

        template <typename F>
        constexpr auto AndThen(F&& func) const noexcept(std::is_nothrow_invocable_v<F>&& std::is_nothrow_copy_constructible_v<E>)
            -> std::invoke_result_t<F>
        {
            using FuncResult = std::invoke_result_t<F>;
            static_assert(Detail::SameErrorTypeAs<FuncResult, E>, "AndThen function must return Result with the same Error type!");

            if (IsError())
            {
                return ErrWrapper<E>{ *m_error };
            }
            return std::invoke(std::forward<F>(func));
        }

        template <typename F>
        constexpr auto MapError(F&& func) const& noexcept(std::is_nothrow_invocable_v<F, const E&>)
            -> Result<void, std::invoke_result_t<F, const E&>>
        {
            using U = std::invoke_result_t<F, const E&>;
            if (IsOk())
            {
                return {};
            }
            return ErrWrapper<U>{ std::invoke(std::forward<F>(func), *m_error) };
        }

        template <typename F>
        constexpr auto MapError(F&& func) && noexcept(std::is_nothrow_invocable_v<F, E&&>)
            -> Result<void, std::invoke_result_t<F, E&&>>
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
            noexcept(std::is_nothrow_invocable_v<OkFunc>&& std::is_nothrow_invocable_v<ErrFunc, const E&>)
        {
            using OkResult = std::invoke_result_t<OkFunc>;
            using ErrResult = std::invoke_result_t<ErrFunc, const E&>;
            static_assert(std::is_same_v<OkResult, ErrResult>, "Match handlers must return the same type!");

            if (IsOk())
            {
                return std::invoke(std::forward<OkFunc>(okFunc));
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), *m_error);
        }

        template <typename OkFunc, typename ErrFunc>
        constexpr auto Match(OkFunc&& okFunc, ErrFunc&& errFunc) &&
            noexcept(std::is_nothrow_invocable_v<OkFunc>&& std::is_nothrow_invocable_v<ErrFunc, E&&>)
        {
            using OkResult = std::invoke_result_t<OkFunc>;
            using ErrResult = std::invoke_result_t<ErrFunc, E&&>;
            static_assert(std::is_same_v<OkResult, ErrResult>, "Match handlers must return the same type!");

            if (IsOk())
            {
                return std::invoke(std::forward<OkFunc>(okFunc));
            }
            return std::invoke(std::forward<ErrFunc>(errFunc), std::move(*m_error));
        }

        [[nodiscard]] NU_FORCEINLINE constexpr bool IsOk() const noexcept 
        { 
            return !m_error.has_value(); 
        }
        [[nodiscard]] NU_FORCEINLINE constexpr bool IsError() const noexcept 
        { 
            return m_error.has_value(); 
        }
        [[nodiscard]] constexpr explicit operator bool() const noexcept 
        { 
            return IsOk(); 
        }

        constexpr bool operator==(const Result& other) const
            noexcept(noexcept(std::declval<const E&>() == std::declval<const E&>()))
            requires std::equality_comparable<E>
        {
            if (IsOk() && other.IsOk())
            {
                return true;
            }
            if (IsError() && other.IsError())
            {
                return *m_error == *other.m_error;
            }
            return false;
        }

        constexpr bool operator!=(const Result& other) const
            noexcept(noexcept(std::declval<Result>() == other))
            requires std::equality_comparable<E>
        {
            return !(*this == other);
        }

        constexpr bool operator==(const OkWrapper<void>&) const noexcept { return IsOk(); }

        template <typename U>
        constexpr bool operator==(const ErrWrapper<U>& err) const
            noexcept(noexcept(std::declval<const E&>() == err.error))
            requires std::equality_comparable_with<E, U>
        {
            return IsError() && *m_error == err.error;
        }

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

        [[nodiscard]] constexpr const E&& UnwrapError() const&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, "Panic! Attempted to UnwrapError() on an Ok result.");
                std::terminate();
            }
            return std::move(*m_error);
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
        [[nodiscard]] constexpr const E&& ExpectError(Msg&& msg) const&&
        {
            if (IsOk())
            {
                NU_ASSERT(false, msg);
                std::terminate();
            }
            return std::move(*m_error);
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

        [[nodiscard]] constexpr const E* TryUnwrapError() const noexcept 
        { 
            return m_error ? &(*m_error) : nullptr;
        }
        [[nodiscard]] constexpr E* TryUnwrapError() noexcept 
        {
            return m_error ? &(*m_error) : nullptr; 
        }

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
} // namespace NuEngine::Core
