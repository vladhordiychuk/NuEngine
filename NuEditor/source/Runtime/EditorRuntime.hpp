#pragma once

#include <QObject>
#include <QTimer>
#include <QLabel>

#include <Runtime/GameLoader.hpp>
#include <Core/EditorState.hpp>
#include <NuEngine/Runtime/Application.hpp>

namespace NuEditor
{
	class EditorRuntime : public QObject
	{
		Q_OBJECT
	public:
		explicit EditorRuntime(QObject* parent = nullptr);
		~EditorRuntime();

		bool LoadDll(const QString& path);
		void UnLoadDll();

		void StartRenderLoop();

		void Play();
		void Pause();
		void Stop();

		[[nodiscard]] EditorState GetState() const { return m_State; }
		[[nodiscard]] bool IsLoaded() const { return m_GameLoader.IsLoaded(); }
		[[nodiscard]] NuEngine::Runtime::Application* GetApp() const { return m_App; }

	signals:
		void StateChanged(EditorState newState);
		void DllLoaded();
		void DllLoadedFailed(const QString& reason);
		void FrameReady(float dt);
		void FpsUpdated(float fps, float ms);
		void AppResetRequired();

	private slots:
		void OnTimerTick();

	private:
		void InitializeApp();
		void DestroyApp();
		void RecreateApp();

		GameLoader m_GameLoader;
		NuEngine::Runtime::Application* m_App = nullptr;
		EditorState m_State = EditorState::Stopped;
		QTimer m_Timer;

		int m_FrameCount = 0;
		float m_FpsTimer = 0.0f;
	};
}