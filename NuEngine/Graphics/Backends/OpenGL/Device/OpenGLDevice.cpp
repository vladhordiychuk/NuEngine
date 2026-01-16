#include <Graphics/Backends/OpenGL/Device/OpenGLDevice.hpp>
#include <glad/glad.h>
#include <NuMath/NuMath.hpp>
#include <cmath>

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
            FragColor = vec4(1.0, 1.0, 1.0, 0.2);
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
        if (m_ShaderProgram) glDeleteProgram(m_ShaderProgram);
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    void OpenGLDevice::CreatePipelineResources()
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
        glCompileShader(fragmentShader);

        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, vertexShader);
        glAttachShader(m_ShaderProgram, fragmentShader);
        glLinkProgram(m_ShaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

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

        glUseProgram(m_ShaderProgram);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);;

        glBindVertexArray(0);
        glUseProgram(0);

        return Core::Ok();
    }

    Core::Result<void, GraphicsError> OpenGLDevice::Present() noexcept
    {
        if (!m_Context) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidContext));
        return m_Context->SwapBuffers();
    }
}