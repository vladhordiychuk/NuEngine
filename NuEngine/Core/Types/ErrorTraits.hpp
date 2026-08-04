// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <concepts>
#include <string_view>

namespace NuEngine::Core
{
    /**
     * @brief Primary template. Must be specialized for each error code enum.
     */
	template <typename T>
	struct ErrorTraits;

    /**
     * @brief Constrains a traits type to expose Category and ToString().
     */
    template <typename Traits, typename CodeEnum>
    concept ErrorTraitsConcept = requires(CodeEnum code)
    {
        { Traits::Category } -> std::convertible_to<std::string_view>;
        { Traits::ToString(code) } -> std::convertible_to<std::string_view>;
    };
} // namespace NuEngine::Core