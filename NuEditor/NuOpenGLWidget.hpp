#pragma once

#include <glad/glad.h>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <NuEngine/Runtime/Application.hpp>

class NuOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    NuOpenGLWidget(NuEngine::Runtime::Application* app, QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    NuEngine::Runtime::Application* m_App;
    QTimer* m_UpdateTimer;
    QElapsedTimer m_ElapsedTimer;

    int QtKeyToKeyCode(int qtKey);
};