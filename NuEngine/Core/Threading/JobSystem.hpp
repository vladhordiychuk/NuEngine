// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <thread>
#include <vector>
#include <functional>

namespace NuEngine::Core
{
	class JobSystem
	{
	public:
		void Initialize();

		void Shutdown();

		void ParallelFor(size_t count, std::function<void(size_t, size_t)> job);

		static JobSystem& Get();

	private:
		std::vector<std::thread> m_workers;
		unsigned int m_numThreads;
	};
}