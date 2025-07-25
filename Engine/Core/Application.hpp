#pragma once 

namespace Engine::Core::Application 
{
    class Application 
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        bool Initialize();
        void Shutdown();
        void MainLoop() const;

        bool m_isRunning = false;
    };
}