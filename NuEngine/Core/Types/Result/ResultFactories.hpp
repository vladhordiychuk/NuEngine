// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <type_traits>
#include <utility>
#include <Core/Platform/CompilerMacros.hpp>
#include <Core/Types/Result/ResultWrappers.hpp>

namespace NuEngine::Core
{
    template <typename T>
    NU_FORCEINLINE constexpr OkWrapper<std::decay_t<T>> Ok(T&& value)
        noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T&&>)
    {
        return { std::forward<T>(value) };
    }

    template <typename E>
    NU_FORCEINLINE constexpr ErrWrapper<std::decay_t<E>> Err(E&& error)
        noexcept(std::is_nothrow_constructible_v<std::decay_t<E>, E&&>)
    {
        return { std::forward<E>(error) };
    }

    NU_FORCEINLINE constexpr OkWrapper<void> Ok() noexcept { return {}; }

    template <typename T, typename... Args>
    NU_FORCEINLINE constexpr OkWrapper<T> MakeOk(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        return { T(std::forward<Args>(args)...) };
    }

    template <typename E, typename... Args>
    NU_FORCEINLINE constexpr ErrWrapper<E> MakeErr(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args...>)
    {
        return { E(std::forward<Args>(args)...) };
    }
} // namespace NuEngine::Core