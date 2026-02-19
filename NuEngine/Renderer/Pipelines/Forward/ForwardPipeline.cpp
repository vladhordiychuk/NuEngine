#include <Renderer/Pipelines/Forward/ForwardPipeline.hpp>
#include <NuMath/NuMath.hpp>
#include <Core/Logging/Logger.hpp>
#include <Core/IO/FileSystem.hpp>
#include <Core/Timer/Time.hpp>
#include <glad/glad.h>
#include <cmath> // Для константи PI, якщо треба

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace NuEngine::Renderer
{
    ForwardPipeline::ForwardPipeline(Graphics::IRenderDevice* device)
        : m_Device(device), m_Width(1280), m_Height(720)
    {
        Initialize();
    }

    void ForwardPipeline::Initialize()
    {
        if (!m_Device)
        {
            LOG_ERROR("ForwardPipeline: Invalid Device Context");
            return;
        }

        Core::FileSystem fs(".");

        std::string vertexSrc;
        auto vertRes = fs.ReadTextFile("Resources/Shaders/Forward.vert");
        if (vertRes.IsOk()) vertexSrc = vertRes.Unwrap();
        else { LOG_ERROR("Failed to load Vertex Shader! Error: {}", vertRes.UnwrapError().ToString()); return; }

        std::string fragmentSrc;
        auto fragRes = fs.ReadTextFile("Resources/Shaders/Forward.frag");
        if (fragRes.IsOk()) fragmentSrc = fragRes.Unwrap();
        else { LOG_ERROR("Failed to load Fragment Shader! Error: {}", fragRes.UnwrapError().ToString()); return; }

        auto shaderRes = m_Device->CreateShader(vertexSrc, fragmentSrc);
        if (shaderRes.IsOk()) m_Shader = shaderRes.Unwrap();
        else LOG_ERROR("Critical: Failed to create shader! Reason: {}", shaderRes.UnwrapError().ToString());

        glEnable(GL_DEPTH_TEST);

        float aspectRatio = (float)m_Width / (float)m_Height;

        // ВИПРАВЛЕНО: Ручне переведення в радіани, якщо NuMath::ToRadians відсутній
        float fovRadians = 45.0f * (static_cast<float>(M_PI) / 180.0f);
        m_Camera = std::make_shared<Camera>(fovRadians, aspectRatio, 0.1f, 100.0f);

        m_Camera->SetPosition(NuMath::Vector3(0.0f, 0.0f, 3.0f));

        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                // ... (решта вершин без змін) ...
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        auto texPath = Core::FileSystem::GetPath("Resources/Textures/wall.jpg");
        auto texture = m_Device->CreateTexture(texPath.generic_string());

        if (texture)
        {
            m_Texture = texture;
            m_Shader->Bind();
            m_Shader->SetInt("u_Texture", 0);
            m_Shader->Unbind();
        }
        else
        {
            LOG_ERROR("Failed to load texture. Tried path: {}", texPath.string());
        }

        m_QuadVAO = m_Device->CreateVertexArray();
        auto vbo = m_Device->CreateVertexBuffer(vertices, sizeof(vertices));
        Graphics::BufferLayout layout = {
            { Graphics::ShaderDataType::Float3, "aPos" },
            { Graphics::ShaderDataType::Float2, "aTexCoord" }
        };
        vbo->SetLayout(layout);
        m_QuadVAO->AddVertexBuffer(vbo);
    }

    Core::Result<void, Graphics::GraphicsError> ForwardPipeline::Render(bool present) noexcept
    {
        if (!m_Device) return Core::Err(Graphics::GraphicsError(Graphics::GraphicsErrorCode::InvalidContext));

        glClearColor(m_ClearColor.R(), m_ClearColor.G(), m_ClearColor.B(), m_ClearColor.A());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_Shader && m_Camera)
        {
            m_Shader->Bind();

            NuMath::Matrix4x4 projection = m_Camera->GetProjectionMatrix();
            NuMath::Matrix4x4 view = m_Camera->GetViewMatrix();

            float animSpeed = 0.5f;
            float angle = Core::Time::GetTimeSinceStartup() * animSpeed;
            NuMath::Quaternion currentRot = NuMath::Quaternion::FromAxisAngle(
                NuMath::Vector3(0.5f, 1.0f, 0.0f).Normalize(),
                angle
            );

            NuMath::Transform meshTransform;
            meshTransform.SetPosition(NuMath::Vector3(0.0f, 0.0f, 0.0f));
            meshTransform.SetRotation(currentRot);

            NuMath::Matrix4x4 model = meshTransform.GetMatrix();

            GLint modelLoc = glGetUniformLocation(m_Shader->GetID(), "model");
            GLint viewLoc = glGetUniformLocation(m_Shader->GetID(), "view");
            GLint projLoc = glGetUniformLocation(m_Shader->GetID(), "projection");

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.Data());
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.Data());
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.Data());
        }

        if (m_Texture) m_Texture->Bind(0);

        if (m_QuadVAO)
        {
            m_QuadVAO->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            m_QuadVAO->Unbind();
        }

        m_Shader->Unbind();

        if (present) return m_Device->Present();

        return Core::Ok();
    }

    void ForwardPipeline::SetViewport(int x, int y, int width, int height) noexcept
    {
        if (m_Device)
        {
            m_Device->SetViewport(x, y, width, height);
            m_Width = width;
            m_Height = height;

            if (m_Camera) m_Camera->SetViewportSize(width, height);
        }
    }
}