// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <chrono>

namespace NuEngine::Core
{
	class Time
	{
	public:
		[[nodiscard]] static float GetDeltaTime();

		[[nodiscard]] static float GetUnscaledDeltaTime();

		[[nodiscard]] static float GetTimeSinceStartup();

		static void SetTimeScale(float scale);

		[[nodiscard]] static float GetTimeScale();

		static void Initialize();

		static void Update();

	private:
		static float s_DeltaTime;
		static float s_TimeScale;
		static float s_TimeSinceStartup;

		static std::chrono::high_resolution_clock::time_point s_StartTime;
		static std::chrono::high_resolution_clock::time_point s_LastFrameTime;
	};
}