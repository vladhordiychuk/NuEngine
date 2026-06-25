#pragma once

#include <glad/glad.h>

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>

#include <NuEngine/Runtime/Application.hpp>

namespace NuEditor
{
    class NuOpenGLWidget : public QOpenGLWidget
    {
        Q_OBJECT
    public:
        NuOpenGLWidget(NuEngine::Runtime::Application* app, QWidget* parent = nullptr);

        void SetApp(NuEngine::Runtime::Application* app) { m_App = app; }
        [[nodiscard]] bool IsGlInitialized() const { return m_GLInitialized; }
        void InitializeApp();

    protected:
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w, int h) override;

        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    signals:
        void glInitialized();

    private:
        NuEngine::Runtime::Application* m_App;

        bool m_GLInitialized = false;

        int QtKeyToKeyCode(int qtKey);
    };
} // namespace NuEditor