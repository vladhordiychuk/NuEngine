#include <QFileInfo>

#include <Runtime/GameLoader.hpp>

#ifdef _WIN32
	#include <Windows.h>
	#define NU_LOAD_LIB(path)		LoadLibraryW(path)
	#define NU_GET_PROC(lib, sym)	GetProcAddress((HMODULE)lib, sym)
	#define NU_FREE_LIB(lib)		FreeLibrary((HMODULE)lib)
#else
	#include <dlfcn.h>
	#define NU_LOAD_LIB(path)		dlopen(path, PTLD_NOW)
	#define NU_GET_PROC(lib, sym)	dlsym(lib, sym)
	#define NU_FREE_LIB(lib)		dlclose(lib)
#endif

namespace NuEditor
{
	bool GameLoader::Load(const QString& dllPath)
	{
		UnLoad();

		if (!QFileInfo::exists(dllPath))
		{
			return false;
		}

#ifdef _WIN32
		m_Handle = (void*)NU_LOAD_LIB(dllPath.toStdWString().c_str());
#else
		m_Handle = NU_LOAD_LIB(dllPath.toStdString().c_str());
#endif

		m_CreateFn = (CreateFn)NU_GET_PROC(m_Handle, "CreateGameApplication");
		m_DestroyFn = (DestroyFn)NU_GET_PROC(m_Handle, "DestroyGameApplication");

		if (!m_CreateFn || !m_DestroyFn)
		{
			UnLoad();
			return false;
		}

		return true;
	}

	void GameLoader::UnLoad()
	{
		if (m_Handle)
		{
			NU_FREE_LIB(m_Handle);
			m_Handle = nullptr;
			m_CreateFn = nullptr;
			m_DestroyFn = nullptr;
		}
	}

	NuEngine::Runtime::Application* GameLoader::CreateApp(NuEngine::Runtime::ApplicationSpecification& spec)
	{
		if (!m_CreateFn)
		{
			return nullptr;
		}
		return m_CreateFn(spec);
	}

	void GameLoader::DestroyApp(NuEngine::Runtime::Application* app)
	{
		if (m_DestroyFn && app)
		{
			m_DestroyFn(app);
		}
	}
} // namespace NuEditor

#undef NU_LOAD_LIB
#undef NU_GET_PROC
#undef NU_FREE_LIB