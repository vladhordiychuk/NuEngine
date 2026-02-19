#include <Core/Timer/Time.hpp>

namespace NuEngine::Core
{
	float Time::s_DeltaTime = 0.0f;
	float Time::s_TimeScale = 1.0f;
	float Time::s_TimeSinceStartup = 0.0f;

	std::chrono::high_resolution_clock::time_point Time::s_StartTime;
	std::chrono::high_resolution_clock::time_point Time::s_LastFrameTime;

	void Time::Initialize()
	{
		s_StartTime = std::chrono::high_resolution_clock::now();
		s_LastFrameTime = s_StartTime;
		s_TimeSinceStartup = 0.0f;
		s_DeltaTime = 0.0f;
	}

	void Time::Update()
	{
		auto currentFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta = currentFrameTime - s_LastFrameTime;
		float rawDelta = delta.count();

		if (rawDelta > 0.1f)
		{
			rawDelta = 0.1f;
		}

		s_DeltaTime = rawDelta * s_TimeScale;

		std::chrono::duration<float> total = currentFrameTime - s_StartTime;
		s_TimeSinceStartup = total.count();

		s_LastFrameTime = currentFrameTime;
	}

	float Time::GetDeltaTime()
	{
		return s_DeltaTime;
	}

	float Time::GetTimeSinceStartup()
	{
		return s_TimeSinceStartup;
	}

	float Time::GetUnscaledDeltaTime()
	{
		if (s_TimeScale == 0.0f)
		{
			return 0.0f;
		}
		return s_DeltaTime / s_TimeScale;
	}

	float Time::GetTimeScale()
	{
		return s_TimeScale;
	}

	void Time::SetTimeScale(float scale)
	{
		if (scale >= 0.0f)
		{
			s_TimeScale = scale;
		}
	}
}