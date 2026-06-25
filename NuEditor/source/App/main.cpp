#include <QApplication>
#include <QTimer>
#include <App/EditorWindow.hpp>
#include <Core/EditorPreferences.hpp>
#include <Core/ThemeManager.hpp>

int main(int argc, char* argv[])
{
    QApplication qtApp(argc, argv);

    NuEditor::EditorPreferences::Get().Load();
    NuEditor::Core::ThemeManager::Get().ApplyCurrent();

    NuEditor::EditorWindow window;
    window.show();

    QTimer::singleShot(0, [&window]() {
        window.ToggleCustomMaximize();
        });

    return qtApp.exec();
}