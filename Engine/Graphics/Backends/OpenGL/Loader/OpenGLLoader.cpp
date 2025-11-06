#include <Graphics/Backends/OpenGL/Loader/OpenGLLoader.hpp>

#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
	Core::Result<void, Core::GraphicsError> OpenGLLoader::LoadFunctions() noexcept
	{
		if (!gladLoadGL())
		{
			return Core::Err(Core::GraphicsError::FunctionLoadFailed);
		}

		const char* version = (const char*)glGetString(GL_VERSION);
		if (!version)
		{
			return Core::Err(Core::GraphicsError::FunctionLoadFailed);
		}

		return Core::Ok<Core::GraphicsError>();
	}
}