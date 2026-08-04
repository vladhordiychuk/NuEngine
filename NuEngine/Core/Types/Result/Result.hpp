// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result/ResultFwd.hpp>
#include <Core/Types/Result/ResultWrappers.hpp>
#include <Core/Types/Result/ResultFactories.hpp>
#include <Core/Types/Result/ResultTraits.hpp>
#include <Core/Types/Result/ResultCommon.hpp>
#include <Core/Types/Result/Details/ResultStorage.hpp>
#include <Core/Types/Result/ResultValue.hpp>
#include <Core/Types/Result/ResultVoid.hpp>
#include <Core/Types/Result/ResultMacros.hpp>

namespace NuEngine::Core
{
    template <typename T, typename E, typename... Args>
    [[nodiscard]] NU_FORCEINLINE constexpr Result<T, E> MakeResultOk(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        return Result<T, E>(std::in_place, std::forward<Args>(args)...);
    }

    template <typename T, typename E, typename... Args>
    [[nodiscard]] NU_FORCEINLINE constexpr Result<T, E> MakeResultErr(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args...>)
    {
        return Result<T, E>(InPlaceErr, std::forward<Args>(args)...);
    }
} // namespace NuEngine::Core