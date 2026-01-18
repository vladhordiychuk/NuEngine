// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Backends/OpenGL/Context/OpenGLContext.hpp>
#include <windows.h>

namespace NuEngine::Graphics::OpenGL
{
    /*
    * @brief
    */
    class OpenGLContextWin32 final : public OpenGLContext
    {
    public:
        /*
        * @brief
        */
        OpenGLContextWin32(Platform::IWindow* window);

        /*
        * @brief
        */
        ~OpenGLContextWin32() override;

        /*
        * @brief
        */
        [[nodiscard]] Core::Result<void, GraphicsError> Initialize() noexcept override;

        /*
        * @brief
        */
        [[nodiscard]] Core::Result<void, GraphicsError> SwapBuffers() noexcept override;

        /*
        * @brief
        */
        [[nodiscard]] Core::Result<void, GraphicsError> MakeCurrent() noexcept override;

    private:
        Platform::IWindow* m_Window;
        HDC m_HDC;
        HGLRC m_Context;
    };
}