#include <Core/Types/Types.hpp>
#include <Platform/Common/WindowEvents.hpp>

namespace NuEngine::Platform
{
    void WindowEventSystem::PushEvent(std::unique_ptr<WindowEvent> event)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Events.push_back(std::move(event));
    }

    std::vector<std::unique_ptr<WindowEvent>> WindowEventSystem::PollEvents()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        std::vector<std::unique_ptr<WindowEvent>> events;
        events.swap(m_Events);
        return events;
    }

    void WindowEventSystem::SetEventCallback(const std::function<void(const WindowEvent&)>& callback)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_EventCallback = callback;
    }

    void WindowEventSystem::ProcessCallbacks()
    {
        std::vector<std::unique_ptr<WindowEvent>> events;

        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            events.swap(m_Events);
        }

        if (m_EventCallback)
        {
            for (auto& event : events)
            {
                m_EventCallback(*event);
            }
        }
    }
}