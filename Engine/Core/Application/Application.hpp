// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

#include <Core/Types/Types.hpp>

namespace NuEngine::Core
{
    using NuBool = NuEngine::Core::Types::NuBool;

    /*
    * 
    */
    class Application 
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        NuBool Initialize();
        NuBool Shutdown();
        NuBool MainLoop() const;

        NuBool m_isRunning = false;
    };
}