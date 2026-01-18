#include <Graphics/Backends/OpenGL/Device/OpenGLDevice.hpp>
#include <Graphics/Backends/OpenGL/Resources/Shader.hpp>
#include <Core/Logging/Logger.hpp>

#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 1.0, 0.2);
        }
    )";

    OpenGLDevice::OpenGLDevice(std::unique_ptr<IGraphicsContext> context)
        : m_Context(std::move(context))
    {
        if (m_Context)
        {
            m_Context->MakeCurrent();
            CreatePipelineResources();
        }
    }

    OpenGLDevice::~OpenGLDevice()
    {
        if (m_VAO)
        {
            glDeleteVertexArrays(1, &m_VAO);
        }

        if (m_VBO)
        {
            glDeleteBuffers(1, &m_VBO);
        }
    }

    Core::Result<std::shared_ptr<IShader>, GraphicsError> OpenGLDevice::CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) noexcept
    {
        auto shader = std::make_shared<Shader>();

        auto initResult = shader->Initialize(vertexSrc, fragmentSrc);

        if (initResult.IsError())
        {
            return Core::Err(initResult.UnwrapError());
        }
        return Core::Ok(std::static_pointer_cast<IShader>(shader));
    }

    void OpenGLDevice::CreatePipelineResources()
    {
        auto shaderRes = CreateShader(vertexShaderSrc, fragmentShaderSrc);

        if (shaderRes.IsOk())
        {
            m_Shader = shaderRes.Unwrap();
        }
        else
        {
            LOG_ERROR("Failed to create default shader: {}", shaderRes.UnwrapError().ToString());
        }

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
             -0.5f, 0.5f, 0.0f,
        };

        unsigned int indicies[] = {
            2, 1, 3,
            1, 0, 3,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Core::Result<void, GraphicsError> OpenGLDevice::Clear(float r, float g, float b, float a) noexcept
    {
        if (!m_Context) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidContext));
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return Core::Ok();
    }

    Core::Result<void, GraphicsError> OpenGLDevice::DrawTriangle() noexcept
    {
        if (!m_Context) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidContext));

        if (m_Shader)
        {
            m_Shader->Bind();
        }

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);;

        glBindVertexArray(0);

        if (m_Shader)
        {
            m_Shader->UnBind();
        }

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