#include <Graphics/Backends/OpenGL/Platform/Windows/OpenGLContextWin32.hpp>
#include <Graphics/Backends/OpenGL/Loader/OpenGLLoader.hpp>
#include <Platform/IWindow.hpp>
#include <Core/Logging/Logger.hpp>

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <string>

#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x00000001
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_TYPE_RGBA_ARB                         0x202B
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_ALPHA_BITS_ARB                        0x201B
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#endif

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hDc, const int* piAttribList, const FLOAT* pfAttribList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

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
        if (!m_Window) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidWindow));

        m_HDC = static_cast<HDC>(m_Window->GetNativeHandle(Platform::NativeHandleType::Display));
        if (!m_HDC) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidWindow));

        WNDCLASSEX dummyWc = {};
        dummyWc.cbSize = sizeof(WNDCLASSEX);
        dummyWc.style = CS_OWNDC;
        dummyWc.lpfnWndProc = DefWindowProc;
        dummyWc.hInstance = GetModuleHandle(nullptr);
        dummyWc.lpszClassName = TEXT("NuEngineDummyWindow");
        RegisterClassEx(&dummyWc);

        HWND dummyHwnd = CreateWindow(TEXT("NuEngineDummyWindow"), TEXT("Dummy"), 0, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
        if (!dummyHwnd) return Core::Err(GraphicsError(GraphicsErrorCode::PlatformFailure));

        HDC dummyDC = GetDC(dummyHwnd);

        PIXELFORMATDESCRIPTOR dummyPfd = {};
        dummyPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        dummyPfd.nVersion = 1;
        dummyPfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        dummyPfd.iPixelType = PFD_TYPE_RGBA;
        dummyPfd.cColorBits = 32;
        dummyPfd.iLayerType = PFD_MAIN_PLANE;

        int dummyFormat = ChoosePixelFormat(dummyDC, &dummyPfd);
        SetPixelFormat(dummyDC, dummyFormat, &dummyPfd);

        HGLRC dummyContext = wglCreateContext(dummyDC);
        if (!dummyContext || !wglMakeCurrent(dummyDC, dummyContext))
        {
            if (dummyContext) wglDeleteContext(dummyContext);
            DestroyWindow(dummyHwnd);
            return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed, "Dummy Context Failed"));
        }

        auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
        auto wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(dummyContext);
        DestroyWindow(dummyHwnd);
        UnregisterClass(TEXT("NuEngineDummyWindow"), GetModuleHandle(nullptr));

        if (!wglCreateContextAttribsARB || !wglChoosePixelFormatARB)
        {
            return Core::Err(GraphicsError(GraphicsErrorCode::UnsupportedFeature, "WGL Extensions not found"));
        }

        const int pixelAttribs[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0
        };

        int pixelFormat;
        UINT numFormats;

        if (!wglChoosePixelFormatARB(m_HDC, pixelAttribs, nullptr, 1, &pixelFormat, &numFormats) || numFormats == 0)
        {
            return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed, "wglChoosePixelFormatARB failed"));
        }

        PIXELFORMATDESCRIPTOR pfd;
        DescribePixelFormat(m_HDC, pixelFormat, sizeof(pfd), &pfd);

        if (!SetPixelFormat(m_HDC, pixelFormat, &pfd))
        {
            DWORD err = GetLastError();
            std::string msg = "SetPixelFormat failed. Error: " + std::to_string(err);
            return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed, msg));
        }

        // Запит версії 4.5
        const int contextAttribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 5,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef _DEBUG
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
            0
        };

        m_Context = wglCreateContextAttribsARB(m_HDC, 0, contextAttribs);

        if (!m_Context)
        {
            // ОТРИМУЄМО КОД ПОМИЛКИ
            DWORD err = GetLastError();
            std::string msg = "wglCreateContextAttribsARB failed. WinError: " + std::to_string(err);

            // Якщо помилка 0xc007000d (Invalid Parameter) - скоріше за все версія або Debug біт
            LOG_CRITICAL("Context Creation Failed. Error Code: {}", err);

            return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed, msg));
        }

        if (!wglMakeCurrent(m_HDC, m_Context))
        {
            DWORD err = GetLastError();
            return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed, "wglMakeCurrent failed: " + std::to_string(err)));
        }

        return Graphics::OpenGL::OpenGLLoader::LoadFunctions();
    }

    Core::Result<void, GraphicsError> OpenGLContextWin32::SwapBuffers() noexcept
    {
        if (!m_Window) return Core::Err(GraphicsError(GraphicsErrorCode::InvalidWindow));
        if (m_Window->SwapBuffers().IsError()) return Core::Err(GraphicsError(GraphicsErrorCode::SwapBuffersFailed));
        return Core::Ok();
    }

    Core::Result<void, GraphicsError> OpenGLContextWin32::MakeCurrent() noexcept
    {
        if (!m_Context) return Core::Err(GraphicsError(GraphicsErrorCode::ContextCreationFailed));
        if (!wglMakeCurrent(m_HDC, m_Context)) return Core::Err(GraphicsError(GraphicsErrorCode::UnsupportedFeature));
        return Core::Ok();
    }
}