#include <Core/Types/Types.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace NuEngine::Core::Internal
{
    void TriggerAssert(const char* condition, const char* file, int line, const char* msg)
    {
        std::cerr << "------------------------------------------------\n"
            << "CRITICAL ASSERTION FAILED\n"
            << "Expr: " << condition << "\n"
            << "File: " << file << "\n"
            << "Line: " << line << "\n";

        if (msg) {
            std::cerr << "Msg:  " << msg << "\n";
        }
        std::cerr << "------------------------------------------------\n";
    }
}