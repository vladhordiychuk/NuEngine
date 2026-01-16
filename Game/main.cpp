#include <NuEngine/NuEngine.hpp>
#include <NuEngine/Runtime/EntryPoint.hpp>
#include <memory>

class SandboxApp : public NuEngine::Runtime::Application
{
public:
    SandboxApp()
    {
        LOG_INFO("Sandbox App started! Hello NuEngine!");
        LOG_WARNING("This is a warning test.");
    }

    ~SandboxApp()
    {
        LOG_INFO("Sandbox App Destroyed!");
    }

    void OnUpdate(float deltaTime) override
    {

    }

    void OnRender() override
    {
    }
};

std::unique_ptr<NuEngine::Runtime::Application> NuEngine::CreateApplication()
{
    return std::make_unique<SandboxApp>();
}