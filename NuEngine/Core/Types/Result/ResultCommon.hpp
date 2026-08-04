// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <type_traits>

namespace NuEngine::Core::Detail
{
    template <typename FuncResult, typename ExpectedError>
    inline constexpr bool SameErrorTypeAs = std::is_same_v<typename FuncResult::error_type, ExpectedError>;
} // namespace NuEngine::Core::Detail