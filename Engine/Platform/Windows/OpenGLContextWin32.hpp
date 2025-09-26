// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/OpenGL/OpenGLContext.hpp>
#include <windows.h>

namespace NuEngine::Platform
{
    class OpenGLContextWin32 final : public NuEngine::Graphics::OpenGL::OpenGLContext
    {
    public:
        OpenGLContextWin32(IWindow* window);
        ~OpenGLContextWin32() override;

        NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> Initialize() override;
        NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> SwapBuffers() override;
        NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> MakeCurrent() override;

    private:
        IWindow* m_Window;
        HDC m_HDC;
        HGLRC m_Context;
    };
}