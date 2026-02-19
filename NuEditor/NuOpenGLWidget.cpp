#include "NuOpenGLWidget.hpp"
#include <NuEngine/Core/Input/Input.hpp>
#include <QMouseEvent>

NuOpenGLWidget::NuOpenGLWidget(NuEngine::Runtime::Application* app, QWidget* parent)
    : QOpenGLWidget(parent), m_App(app)
{
    setFocusPolicy(Qt::StrongFocus);

    m_UpdateTimer = new QTimer(this);
    connect(m_UpdateTimer, &QTimer::timeout, this, [this]() {
        update();
        });
    m_UpdateTimer->start(0);
    m_ElapsedTimer.start();
}

void NuOpenGLWidget::initializeGL()
{
    gladLoadGLLoader((GLADloadproc)([](const char* name) {
        return (void*)QOpenGLContext::currentContext()->getProcAddress(name);
        }));

    if (m_App)
    {
        m_App->InitializeGraphicsForEditor();
    }
}

void NuOpenGLWidget::paintGL()
{
    if (m_App)
    {
        float deltaTime = m_ElapsedTimer.elapsed() / 1000.0f;
        m_ElapsedTimer.restart();

        m_App->UpdateFrame(deltaTime);
        m_App->RenderFrame();
    }
}

void NuOpenGLWidget::resizeGL(int w, int h)
{
    if (m_App)
    {
        m_App->OnWindowResize(w, h);
    }
}

void NuOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        int keyCode = QtKeyToKeyCode(event->key());
        NuEngine::Core::Input::TransitionPressed(static_cast<NuEngine::Core::KeyCode>(keyCode));
    }
    QOpenGLWidget::keyPressEvent(event);
}

void NuOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        int keyCode = QtKeyToKeyCode(event->key());
        NuEngine::Core::Input::TransitionReleased(static_cast<NuEngine::Core::KeyCode>(keyCode));
    }
    QOpenGLWidget::keyReleaseEvent(event);
}

void NuOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    NuEngine::Core::Input::UpdateMouse((float)event->x(), (float)event->y());
    QOpenGLWidget::mouseMoveEvent(event);
}

void NuOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    int button = 0;
    if (event->button() == Qt::LeftButton) button = 0;
    else if (event->button() == Qt::RightButton) button = 1;
    else if (event->button() == Qt::MiddleButton) button = 2;

    NuEngine::Core::Input::UpdateMouseButton(static_cast<NuEngine::Core::KeyCode>(button), true);
    QOpenGLWidget::mousePressEvent(event);
}

void NuOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int button = 0;
    if (event->button() == Qt::LeftButton) button = 0;
    else if (event->button() == Qt::RightButton) button = 1;
    else if (event->button() == Qt::MiddleButton) button = 2;

    NuEngine::Core::Input::UpdateMouseButton(static_cast<NuEngine::Core::KeyCode>(button), false);
    QOpenGLWidget::mouseReleaseEvent(event);
}

int NuOpenGLWidget::QtKeyToKeyCode(int qtKey)
{
    if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
        return qtKey;

    switch (qtKey)
    {
        case Qt::Key_Space: 
            return 32;
        case Qt::Key_Left: 
            return 263;
        case Qt::Key_Right: 
            return 262;
        case Qt::Key_Up: 
            return 265;
        case Qt::Key_Down: 
            return 264;
        case Qt::Key_Shift: 
            return 340;
        case Qt::Key_Control: 
            return 341;
        case Qt::Key_Alt: 
            return 342;
        case Qt::Key_Escape: 
            return 256;
        case Qt::Key_Enter:
        case Qt::Key_Return: 
            return 257;
        default: 
            return qtKey;
    }
}