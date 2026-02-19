#include <Graphics/Backends/OpenGL/Textures/OpenGLTexture.hpp>
#include <Core/Logging/Logger.hpp>
#include <stb/stb_image.h>

namespace NuEngine::Graphics::OpenGL
{
	OpenGLTexture::OpenGLTexture(const std::string& path)
		: m_Path(path), m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0)
	{
		Initialize();
	}

	OpenGLTexture::~OpenGLTexture()
	{
		if (m_RendererID)
		{
			glDeleteTextures(1, &m_RendererID);
		}
	}

	Core::Result<void, GraphicsError> OpenGLTexture::Initialize()
	{
		stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            return Core::Err(GraphicsError(GraphicsErrorCode::ResourceLoadFailed, "Failed to load texture: " + m_Path));
        }

        m_Width = width;
        m_Height = height;
        m_BPP = channels;

        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }
        else
        {
            stbi_image_free(data);
            return Core::Err(GraphicsError(GraphicsErrorCode::InvalidParameter, "Unsupported texture format (channels must be 3 or 4)"));
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        LOG_INFO("Texture loaded successfully: {} ({}x{})", m_Path, m_Width, m_Height);
        return Core::Ok();
	}

    void OpenGLTexture::Bind(unsigned int slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

    void OpenGLTexture::Unbind() const
    {
        glBindTextureUnit(0, 0);
    }
}