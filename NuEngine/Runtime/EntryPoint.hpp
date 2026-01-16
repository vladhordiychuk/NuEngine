// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Runtime/Application.hpp>
#include <memory>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace NuEngine
{
    extern std::unique_ptr<Runtime::Application> CreateApplication();
}

#if defined(NU_PLATFORM_WINDOWS)

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    auto app = NuEngine::CreateApplication();

    auto result = app->Run();

    if (result.IsError())
    {
        auto err = result.UnwrapError();
        MessageBoxA(nullptr, err.ToString().c_str(), "NuEngine Fatal Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    return 0;
}

#else

int main(int argc, char** argv)
{
    auto app = NuEngine::CreateApplication();

    auto result = app->Run();

    if (result.IsError())
    {
        std::cerr << "[FATAL] " << result.UnwrapError().ToString() << std::endl;
        return -1;
    }

    return 0;
}
#endif