#pragma once

#include <QString>

#include <NuEngine/Runtime/Application.hpp>

namespace NuEditor
{
	class GameLoader
	{
	public:
		using CreateFn = NuEngine::Runtime::Application* (*)(const NuEngine::Runtime::ApplicationSpecification&);
		using DestroyFn = void(*)(NuEngine::Runtime::Application*);

		GameLoader() = default;
		~GameLoader() { UnLoad(); }

		[[nodiscard]] bool Load(const QString& dllPath);
		void UnLoad();
		[[nodiscard]] bool IsLoaded() const { return m_Handle != nullptr; }

		[[nodiscard]] NuEngine::Runtime::Application* CreateApp(NuEngine::Runtime::ApplicationSpecification& spec);
		void DestroyApp(NuEngine::Runtime::Application* app);

	private:
		void* m_Handle = nullptr;
		CreateFn m_CreateFn = nullptr;
		DestroyFn m_DestroyFn = nullptr;
	};
} // namespace NuEditor