// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#ifndef NU_EDITOR_MODE

#include <Runtime/Application.hpp>
#include <memory>
#include <iostream>
#include <format>

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
        std::string errStr = std::format("{}", result.UnwrapError());
        MessageBoxA(nullptr, errStr.c_str(), "NuEngine Fatal Error", MB_OK | MB_ICONERROR);
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
        std::cerr << "[FATAL] " << result.UnwrapError() << std::endl;
        return -1;
    }

    return 0;
}
#endif
#endif