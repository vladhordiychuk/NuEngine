#include <Core/Timer/Time.hpp>

namespace NuEngine::Core
{
    Timer::Timer()
    {
        Reset();
    }

    void Timer::Reset()
    {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_lastTime = m_startTime;
    }

    NuFloat Timer::GetDeltaTime()
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<NuFloat> delta = now - m_lastTime;
        m_lastTime = now;
        return delta.count();
    }

    NuFloat Timer::GetElapsedTime() const
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<NuFloat> elapsed = now - m_startTime;
        return elapsed.count();
    }
}
