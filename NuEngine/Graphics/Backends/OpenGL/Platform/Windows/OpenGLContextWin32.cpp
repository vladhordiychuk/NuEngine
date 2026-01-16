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

	Core::Result<void, GraphicsError> OpenGLContextWin32::Initialize() noexcept
	{
		if (!m_Window)
		{
			// ВИПРАВЛЕНО: Обгортаємо код помилки в структуру GraphicsError
			return Core::Err(GraphicsError(GraphicsErrorCode::InvalidWindow));
		}

		m_HDC = static_cast<HDC>(m_Window->GetNativeHandle(Platform::NativeHandleType::Display));
		if (!m_HDC)
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::InvalidWindow));
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
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));

		if (!SetPixelFormat(m_HDC, pixelFormat, &pfd))
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));

		m_Context = wglCreateContext(m_HDC);
		if (!m_Context)
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));
		}

		if (!wglMakeCurrent(m_HDC, m_Context))
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));
		}

		return Graphics::OpenGL::OpenGLLoader::LoadFunctions();
	}

	Core::Result<void, GraphicsError> OpenGLContextWin32::SwapBuffers() noexcept
	{
		if (!m_Window)
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::InvalidWindow));
		}

		auto result = m_Window->SwapBuffers();
		if (result.IsError())
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::SwapBuffersFailed));
		}

		// ВИПРАВЛЕНО: Додано {}
		return Core::Ok();
	}

	Core::Result<void, GraphicsError> OpenGLContextWin32::MakeCurrent() noexcept
	{
		if (!m_Context)
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));
		}

		if (!wglMakeCurrent(m_HDC, m_Context))
		{
			// ВИПРАВЛЕНО
			return Core::Err(GraphicsError(GraphicsErrorCode::UnsupportedFeature));
		}

		// ВИПРАВЛЕНО: Додано {}
		return Core::Ok();
	}
}