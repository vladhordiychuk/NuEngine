#include <Runtime/EditorRuntime.hpp>
#include <NuEngine/Core/Timer/Time.hpp>

namespace NuEditor
{
	EditorRuntime::EditorRuntime(QObject* parent)
	{
		connect(&m_Timer, &QTimer::timeout, this, &EditorRuntime::OnTimerTick);
	}

	EditorRuntime::~EditorRuntime()
	{
		m_Timer.stop();
		DestroyApp();
	}

	bool EditorRuntime::LoadDll(const QString& path)
	{
		m_Timer.stop();
		DestroyApp();
		m_GameLoader.UnLoad();

		if (!m_GameLoader.Load(path))
		{
			emit DllLoadedFailed("Failed to load: " + path);
			return false;
		}

		NuEngine::Runtime::ApplicationSpecification spec;
		spec.Windowed = false;
		spec.Name = "NuEditor";

		m_App = m_GameLoader.CreateApp(spec);

		if (!m_App)
		{
			emit DllLoadedFailed("Failed to create Application from DLL");
			return false;
		}

		emit DllLoaded();
		StartRenderLoop();
		return true;
	}

	void EditorRuntime::UnLoadDll()
	{
		m_Timer.stop();
		DestroyApp();
		m_GameLoader.UnLoad();
	}

	void EditorRuntime::StartRenderLoop()
	{
		m_Timer.start(0);
	}

	void EditorRuntime::Play()
	{
		if (!m_App)
		{
			return;
		}

		m_State = EditorState::Playing;
		emit StateChanged(m_State);
	}

	void EditorRuntime::Pause()
	{
		if (m_State != EditorState::Playing)
		{
			return;
		}
		m_State = EditorState::Paused;
		emit StateChanged(m_State);
	}

	void EditorRuntime::RecreateApp()
	{
		if (m_App)
		{
			NuEngine::Runtime::ApplicationSpecification spec;
			spec.Windowed = false;
			spec.Name = "NuEditor";

			m_GameLoader.DestroyApp(m_App);
			m_App = m_GameLoader.CreateApp(spec);

			emit AppResetRequired();
		}
	}

	void EditorRuntime::Stop()
	{
		if (m_State == EditorState::Stopped)
		{
			return;
		}

		m_State = EditorState::Stopped;
		emit StateChanged(m_State);

		RecreateApp();
	}

	void EditorRuntime::OnTimerTick()
	{
		NuEngine::Core::Time::Update();
		float dt = NuEngine::Core::Time::GetDeltaTime();

		m_FrameCount++;
		m_FpsTimer += dt;

		if (m_FpsTimer >= 1.0f)
		{
			float fps = m_FrameCount / m_FpsTimer;
			emit FpsUpdated(fps, 1000.0f / fps);
			m_FrameCount = 0;
			m_FpsTimer = 0.0f;
		}

		if (m_App && m_State == EditorState::Playing)
		{
			m_App->UpdateFrame(dt);
		}

		emit FrameReady(dt);
	}

	void EditorRuntime::InitializeApp()
	{

	}

	void EditorRuntime::DestroyApp()
	{
		if (m_App)
		{
			m_GameLoader.DestroyApp(m_App);
			m_App = nullptr;
		}
	}
}