// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

#include "../../Graphics/OpenGL/Mesh/Mesh.hpp"
#include "../../Graphics/OpenGL/Shaders/Shader.hpp"

#include "../Logging/Logger.hpp"
#include "../../Platform/Window.hpp"

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