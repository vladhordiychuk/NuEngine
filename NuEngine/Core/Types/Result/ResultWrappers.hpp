// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

namespace NuEngine::Core
{
    template <typename T>
    struct OkWrapper
    {
        T value;
    };

    template <typename E>
    struct ErrWrapper
    {
        E error;
    };

    template <>
    struct OkWrapper<void> {};

    struct InPlaceErr_t 
    { 
        explicit InPlaceErr_t() = default; 
    };
    inline constexpr InPlaceErr_t InPlaceErr{};
} // namespace NuEngine::Core
