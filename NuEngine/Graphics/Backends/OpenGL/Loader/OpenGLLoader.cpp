#include <Graphics/Backends/OpenGL/Loader/OpenGLLoader.hpp>

#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
	Core::Result<void, GraphicsError> OpenGLLoader::LoadFunctions() noexcept
	{
		if (!gladLoadGL())
		{
			return Core::Err(GraphicsError(GraphicsErrorCode::FunctionLoadFailed));
		}

		const char* version = (const char*)glGetString(GL_VERSION);
		if (!version)
		{
			return Core::Err(GraphicsError(GraphicsErrorCode::FunctionLoadFailed));
		}

		return Core::Ok();
	}
}