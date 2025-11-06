#include <Graphics/Backends/OpenGL/Device/OpenGLDevice.hpp>
#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
    OpenGLDevice::OpenGLDevice(std::unique_ptr<IGraphicsContext> context)
        : m_Context(std::move(context))
    {
        if (m_Context)
        {
            m_Context->MakeCurrent();
        }
    }

    Core::Result<void, Core::GraphicsError> OpenGLDevice::Clear(NuFloat r, NuFloat g, NuFloat b, NuFloat a) noexcept
    {
        if (!m_Context) return Core::Err(Core::GraphicsError::InvalidContext);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return Core::Ok<Core::GraphicsError>();
    }

    Core::Result<void, Core::GraphicsError> OpenGLDevice::DrawTriangle() noexcept
    {
        return Core::Ok<Core::GraphicsError>();
    }

    Core::Result<void, Core::GraphicsError> OpenGLDevice::Present() noexcept
    {
        if (!m_Context)
        {
            return Core::Err(Core::GraphicsError::InvalidContext);
        }

        return m_Context->SwapBuffers();
    }
}
