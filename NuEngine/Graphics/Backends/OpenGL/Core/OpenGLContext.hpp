// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Platform/IWindow.hpp>
#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>
#include <Graphics/Abstractions/Core/IGraphicsContext.hpp>
#include <memory>

namespace NuEngine::Graphics::OpenGL
{
    class OpenGLContext : public IGraphicsContext
    {
    public:
        virtual ~OpenGLContext() = default;

        [[nodiscard]] Core::Result<void, GraphicsError> Initialize() noexcept override = 0;
        [[nodiscard]] Core::Result<void, GraphicsError> SwapBuffers() noexcept override = 0;
        [[nodiscard]] Core::Result<void, GraphicsError> MakeCurrent() noexcept override = 0;
    };

    Core::Result<std::unique_ptr<OpenGLContext>, GraphicsError> CreatePlatformOpenGLContext(Platform::IWindow* window) noexcept;
}