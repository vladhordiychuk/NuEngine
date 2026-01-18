#include <Graphics/Backends/OpenGL/Loader/OpenGLLoader.hpp>
#include <glad/glad.h>
#include <windows.h>

namespace NuEngine::Graphics::OpenGL
{
	void* GetAnyGLFuncAddress(const char* name)
	{
		void* p = (void*)wglGetProcAddress(name);

		if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1))
		{
			static HMODULE module = LoadLibraryA("opengl32.dll");
			p = (void*)GetProcAddress(module, name);
		}

		return p;
	}

	Core::Result<void, GraphicsError> OpenGLLoader::LoadFunctions() noexcept
	{
		if (!gladLoadGLLoader((GLADloadproc)GetAnyGLFuncAddress))
		{
			return Core::Err(GraphicsError(GraphicsErrorCode::FunctionLoadFailed, "Failed to initialize GLAD"));
		}

		if (glGetString(GL_VERSION) == nullptr)
		{
			return Core::Err(GraphicsError(GraphicsErrorCode::FunctionLoadFailed, "OpenGL context invalid"));
		}

		return Core::Ok();
	}
}