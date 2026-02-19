#include <QApplication>
#include "EditorWindow.hpp"
#include <NuEngine/Runtime/Application.hpp>

class EditorApp : public NuEngine::Runtime::Application
{
public:
    EditorApp(const NuEngine::Runtime::ApplicationSpecification& spec)
        : Application(spec) 
    {
    }
};

int main(int argc, char* argv[])
{
    QApplication qtApp(argc, argv);

    NuEngine::Runtime::ApplicationSpecification spec;
    spec.Windowed = false;
    spec.Name = "NuEngine Editor";

    EditorApp* engineApp = new EditorApp(spec);

    EditorWindow window(engineApp);
    window.resize(1280, 720);
    window.show();

    int result = qtApp.exec();

    delete engineApp;
    return result;
}