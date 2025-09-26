// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Platform/IWindow.hpp>
#include <Core/Types/Result.hpp>
#include <Core/Errors/GraphicsError.hpp>
#include <memory>

namespace NuEngine::Graphics::OpenGL
{
    class OpenGLContext
    {
    public:
        virtual ~OpenGLContext() = default;

        virtual NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> Initialize() = 0;
        virtual NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> SwapBuffers() = 0;
        virtual NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> MakeCurrent() = 0;
    };

    NuEngine::Core::Result<std::unique_ptr<OpenGLContext>, NuEngine::Core::GraphicsError> CreatePlatformOpenGLContext(Platform::IWindow* window) noexcept;
}
