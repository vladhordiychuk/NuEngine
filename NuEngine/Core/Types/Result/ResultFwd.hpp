// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <type_traits>

namespace NuEngine::Core
{
    template <typename T, typename E>
    class Result;

    namespace Detail
    {
        template <typename T, typename E,
            bool = (std::is_trivially_copyable_v<T>&& std::is_trivially_copyable_v<E>)>
        struct ResultStorage;
    } // namespace Detail
} // namespace NuEngine::Core
