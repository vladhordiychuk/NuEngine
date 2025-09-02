#include <Platform/Windows/WindowWin32.hpp>
#include <thread>

int main()
{
    using namespace NuEngine::Platform;

    WindowWin32 window;
    WindowConfig config(800, 600, "Test Window");

    if (!window.Initialize(config).IsOk())
        return -1;

    window.Show();

    while (window.IsOpen())
    {
        window.ProcessEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}


