#include <Graphics/Backends/OpenGL/Device/OpenGLDevice.hpp>
#include <Graphics/Backends/OpenGL/Resources/OpenGLShader.hpp>
#include <Core/Logging/Logger.hpp>
#include <NuMath/Algebra/Color/Color.hpp>

#include <Graphics/Backends/OpenGL/Resources/OpenGLVertexArray.hpp>
#include <Graphics/Backends/OpenGL/Resources/OpenGLVertexBuffer.hpp>
#include <Graphics/Backends/OpenGL/Resources/OpenGLIndexBuffer.hpp>
#include <Graphics/Backends/OpenGL/Resources/OpenGLTexture.hpp>

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

    Core::Result<std::shared_ptr<IShader>, GraphicsError> OpenGLDevice::CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) noexcept
    {
        auto shader = std::make_shared<OpenGLShader>();

        auto initResult = shader->Initialize(vertexSrc, fragmentSrc);

        if (initResult.IsError())
        {
            return Core::Err(initResult.UnwrapError());
        }
        return Core::Ok(std::static_pointer_cast<IShader>(shader));
    }

    Core::Result<void, GraphicsError> OpenGLDevice::Clear(float r, float g, float b, float a) noexcept
    {
        if (!m_Context) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidContext));
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return Core::Ok();
    }

    std::shared_ptr<IVertexArray> OpenGLDevice::CreateVertexArray()
    {
        return std::make_shared<OpenGLVertexArray>();
    }

    std::shared_ptr<IVertexBuffer> OpenGLDevice::CreateVertexBuffer(float* vertices, unsigned int size)
    {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    std::shared_ptr<IIndexBuffer> OpenGLDevice::CreateIndexBuffer(unsigned int* indices, unsigned int count)
    {
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }

    std::shared_ptr<ITexture> OpenGLDevice::CreateTexture(const std::string& path)
    {
        return std::make_shared<OpenGLTexture>(path);
    }

    Core::Result<void, GraphicsError> OpenGLDevice::DrawIndices(const std::shared_ptr<IVertexArray>& vertexArray) noexcept
    {
        if (!m_Context)
        {
            return Core::Err(GraphicsError(GraphicsErrorCode::InvalidContext));
        }

        if (!vertexArray)
        {
            LOG_WARNING("Attempting to draw null vertex array");
            return Core::Ok();
        }

        vertexArray->Bind();
        uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();

        return Core::Ok();
    }

    Core::Result<void, GraphicsError> OpenGLDevice::Present() noexcept
    {
        if (!m_Context) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidContext));
        return m_Context->SwapBuffers();
    }

    void OpenGLDevice::SetViewport(int x, int y, int width, int height) noexcept
    {
        glViewport(x, y, width, height);
    }
}