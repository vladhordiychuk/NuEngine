#include <Core/Threading/JobSystem.hpp>

namespace NuEngine::Core
{
	void JobSystem::Initialize()
	{
		unsigned int hwThreads = std::thread::hardware_concurrency();
		m_numThreads = (hwThreads != 0) ? hwThreads : 2;

		m_workers.reserve(m_numThreads - 1);
	}

	void JobSystem::Shutdown()
	{
		if (!m_workers.empty())
		{
			for (auto& t : m_workers)
			{
				if (t.joinable())
				{
					t.join();
				}
			}
			m_workers.clear();
		}
	}

	JobSystem& JobSystem::Get()
	{
		static JobSystem instance;
		return instance;
	}

	void JobSystem::ParallelFor(size_t count, std::function<void(size_t, size_t)> job)
	{
		constexpr int THRESHOLD = 1000;
		if (count < THRESHOLD || m_numThreads <= 1)
		{
			job(0, count);
			return;
		}

		unsigned int chunkSize = count / m_numThreads;

		size_t startIndex = 0;

		for (unsigned int i = 0; i < m_numThreads - 1; ++i)
		{
			size_t endIndex = startIndex + chunkSize;

			m_workers.emplace_back([job, startIndex, endIndex]()
			{
				job(startIndex, endIndex);
			});

			startIndex = endIndex;
		}

		job(startIndex, count);

		for (auto& t : m_workers)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
		m_workers.clear();
	}
}