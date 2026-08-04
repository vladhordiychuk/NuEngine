// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#include <Core/Diagnostics/Assert.hpp>
#include <iostream>

namespace NuEngine::Core::Internal
{
    void TriggerAssert(const char* condition, const char* file, int line, const char* msg)
    {
        std::cerr << "------------------------------------------------\n"
            << "CRITICAL ASSERTION FAILED\n"
            << "Expr: " << condition << "\n"
            << "File: " << file << "\n"
            << "Line: " << line << "\n";
        if (msg)
        {
            std::cerr << "Msg:  " << msg << "\n";
        }
        std::cerr << "------------------------------------------------\n";
    }
} // namespace NuEngine::Core::Internal
