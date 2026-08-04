// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <utility>
#include <Core/Types/ErrorContext.hpp>
#include <Core/Types/Result/ResultFactories.hpp>

#define NU_TRY(var, expr)                                        \
    auto var##_result_ = (expr);                                 \
    if (var##_result_.IsError()) [[unlikely]]                    \
    {                                                            \
        auto _err_ = std::move(var##_result_.UnwrapError());     \
        _err_.AddFrame(NuEngine::Core::ErrorContext{});           \
        return NuEngine::Core::Err(std::move(_err_));             \
    }                                                             \
    auto var = std::move(var##_result_.Unwrap())

#define NU_TRY_VOID(expr)                                        \
    do {                                                         \
        auto _r_ = (expr);                                       \
        if (_r_.IsError()) [[unlikely]]                          \
        {                                                        \
            auto _err_ = std::move(_r_.UnwrapError());           \
            _err_.AddFrame(NuEngine::Core::ErrorContext{});        \
            return NuEngine::Core::Err(std::move(_err_));          \
        }                                                          \
    } while(0)
