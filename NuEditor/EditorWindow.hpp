#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel> 
#include "NuOpenGLWidget.hpp"
#include <NuEngine/Runtime/Application.hpp>

QT_BEGIN_NAMESPACE
namespace Ui 
{
    class EditorWindow;
}
QT_END_NAMESPACE

class EditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditorWindow(NuEngine::Runtime::Application* engineApp, QWidget* parent = nullptr);
    ~EditorWindow();

private:
    Ui::EditorWindow* ui;
    NuEngine::Runtime::Application* m_EngineApp;
    QTimer m_Timer;
    NuOpenGLWidget* m_Viewport;

    QLabel* m_FpsLabel;
    int m_FrameCount = 0;
    float m_FpsTimer = 0.0f;

    void UpdateGameLoop();
};
#endif // EDITORWINDOW_H
