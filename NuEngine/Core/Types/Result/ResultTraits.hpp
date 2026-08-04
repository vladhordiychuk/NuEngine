// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <type_traits>
#include <Core/Types/Result/ResultFwd.hpp>

namespace NuEngine::Core
{
    template <typename T>
    struct is_result : std::false_type {};

    template <typename T, typename E>
    struct is_result<Result<T, E>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_result_v = is_result<T>::value;

    template <typename R>
    concept ResultLike = is_result_v<std::remove_cvref_t<R>>;
} // namespace NuEngine::Core
