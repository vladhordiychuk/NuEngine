#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Core/Errors/WindowError.hpp>
#include <Core/Errors/FileSystemError.hpp>

int main()
{
    auto initResult = NuEngine::Core::Logger::Init("logs/app.log");
    if (initResult.IsError())
    {
        std::cerr << "Logger init failed: " << NuEngine::Core::ToString(initResult.UnwrapError()) << std::endl;
        return -1;
    }
    LOG_INFO("Starting NuEngine...");

    NuEngine::Core::Application app;
    auto result = app.Run();
    if (result.IsError())
    {
        LOG_ERROR("Application run failed: {}", NuEngine::Core::ToString(result.UnwrapError()));
        NuEngine::Core::Logger::Shutdown();
        return -1;
    }

    NuEngine::Core::Logger::Shutdown();
    return 0;
}