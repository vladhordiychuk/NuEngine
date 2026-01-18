#include <Core/Types/Types.hpp>
#include <Platform/Common/WindowEvents.hpp>

namespace NuEngine::Platform
{
    void WindowEventSystem::PushEvent(std::unique_ptr<WindowEvent> event)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        
        if (m_EventCallback)
        {
            m_EventCallback(*event);
        }
        m_Events.push_back(std::move(event));
    }

    std::vector<std::unique_ptr<WindowEvent>> WindowEventSystem::PollEvents()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        std::vector<std::unique_ptr<WindowEvent>> events;
        events.swap(m_Events);
        return events;
    }

    void WindowEventSystem::SetEventCallback(const EventCallbackFn& callback)
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