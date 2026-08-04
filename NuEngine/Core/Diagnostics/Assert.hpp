// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/API.hpp>
#include <Core/Platform/CompilerMacros.hpp>

namespace NuEngine::Core::Internal
{
    NU_API void TriggerAssert(const char* condition, const char* file, int line, const char* msg = nullptr);
} // namespace NuEngine::Core::Internal

#if !defined(NDEBUG) || defined(NU_ENABLE_ASSERTS)
    #define NU_ASSERT(expr, msg) \
        if (!(expr)) { \
            NuEngine::Core::Internal::TriggerAssert(#expr, __FILE__, __LINE__, msg); \
            NU_DEBUG_BREAK(); \
        }
#else
    #define NU_ASSERT(expr, msg) ((void)0)
#endif
