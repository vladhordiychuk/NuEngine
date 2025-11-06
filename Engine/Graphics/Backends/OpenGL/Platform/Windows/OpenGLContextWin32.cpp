#include <Graphics/Backends/OpenGL/Platform/Windows/OpenGLContextWin32.hpp>
#include <Graphics/Backends/OpenGL/Loader/OpenGLLoader.hpp>
#include <Platform/IWindow.hpp> 

namespace NuEngine::Graphics::OpenGL
{
	OpenGLContextWin32::OpenGLContextWin32(Platform::IWindow* window)
		: m_Window(window)
		, m_HDC(nullptr)
		, m_Context(nullptr)
	{
	}

	OpenGLContextWin32::~OpenGLContextWin32()
	{
		if (m_Context)
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(m_Context);
		}
	}

	Core::Result<void, Core::GraphicsError> OpenGLContextWin32::Initialize() noexcept
	{
		if (!m_Window)
		{
			return Core::Err(Core::GraphicsError::InvalidWindow);
		}

		m_HDC = static_cast<HDC>(m_Window->GetNativeHandle(Platform::NativeHandleType::Display));
		if (!m_HDC)
		{
			return Core::Err(Core::GraphicsError::InvalidWindow);
		}

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(m_HDC, &pfd);
		if (pixelFormat == 0)
			return Err(Core::GraphicsError::ContextCreationFailed);

		if (!SetPixelFormat(m_HDC, pixelFormat, &pfd))
			return Err(Core::GraphicsError::ContextCreationFailed);

		m_Context = wglCreateContext(m_HDC);
		if (!m_Context)
		{
			return Core::Err(Core::GraphicsError::ContextCreationFailed);
		}

		if (!wglMakeCurrent(m_HDC, m_Context))
		{
			return Core::Err(Core::GraphicsError::ContextCreationFailed);
		}

		return Graphics::OpenGL::OpenGLLoader::LoadFunctions();
	}

	Core::Result<void, Core::GraphicsError> OpenGLContextWin32::SwapBuffers() noexcept
	{
		if (!m_Window)
		{
			return Core::Err(Core::GraphicsError::InvalidWindow);
		}

		auto result = m_Window->SwapBuffers();
		if (result.IsError())
		{
			return NuEngine::Core::Err(NuEngine::Core::GraphicsError::SwapBuffersFailed);
		}

		return NuEngine::Core::Ok<NuEngine::Core::GraphicsError>();
	}

	NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> OpenGLContextWin32::MakeCurrent() noexcept
	{
		if (!m_Context)
		{
			return NuEngine::Core::Err(NuEngine::Core::GraphicsError::ContextCreationFailed);
		}

		if (!wglMakeCurrent(m_HDC, m_Context))
		{
			return NuEngine::Core::Err(NuEngine::Core::GraphicsError::UnsupportedFeature);
		}

		return NuEngine::Core::Ok<NuEngine::Core::GraphicsError>();
	}

}