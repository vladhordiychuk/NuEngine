#include <Graphics/OpenGL/OpenGLLoader.hpp>

#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
	NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> OpenGLLoader::LoadFunctions() noexcept
	{
		if (!gladLoadGL())
		{
			return NuEngine::Core::Err(NuEngine::Core::GraphicsError::FunctionLoadFailed);
		}

		const char* version = (const char*)glGetString(GL_VERSION);
		if (!version)
		{
			return NuEngine::Core::Err(NuEngine::Core::GraphicsError::FunctionLoadFailed);
		}

		return NuEngine::Core::Ok<NuEngine::Core::GraphicsError>();
	}
}