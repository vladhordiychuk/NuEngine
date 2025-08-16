// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

namespace NuEngine::Core::Application 
{
    class Application 
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        bool Initialize();
        void Shutdown();
        void MainLoop() const;

        bool m_isRunning = false;
    };
}