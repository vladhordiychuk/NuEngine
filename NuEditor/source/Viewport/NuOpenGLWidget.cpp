#include <QMouseEvent>

#include <NuOpenGLWidget.hpp>
#include <NuEngine/Core/Input/Input.hpp>

namespace NuEditor
{
    NuOpenGLWidget::NuOpenGLWidget(NuEngine::Runtime::Application* app, QWidget* parent)
        : QOpenGLWidget(parent), m_App(app)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    void NuOpenGLWidget::initializeGL()
    {
        gladLoadGLLoader((GLADloadproc)([](const char* name) {
                return (void*)QOpenGLContext::currentContext()->getProcAddress(name);
            }));

        m_GLInitialized = true;

        if (m_App)
        {
            m_App->InitializeGraphicsForEditor();
            m_App->OnInit();
        }

        emit glInitialized();
    }

    void NuOpenGLWidget::paintGL()
    {
        if (m_App)
        {
            m_App->RenderFrame();
        }
    }

    void NuOpenGLWidget::resizeGL(int w, int h)
    {
        if (m_App)
            m_App->ResizeViewport(w, h);
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
        if (event->button() == Qt::LeftButton)       button = 0;
        else if (event->button() == Qt::RightButton)  button = 1;
        else if (event->button() == Qt::MiddleButton) button = 2;
        NuEngine::Core::Input::UpdateMouseButton(static_cast<NuEngine::Core::KeyCode>(button), true);
        QOpenGLWidget::mousePressEvent(event);
    }

    void NuOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        int button = 0;
        if (event->button() == Qt::LeftButton)       button = 0;
        else if (event->button() == Qt::RightButton)  button = 1;
        else if (event->button() == Qt::MiddleButton) button = 2;
        NuEngine::Core::Input::UpdateMouseButton(static_cast<NuEngine::Core::KeyCode>(button), false);
        QOpenGLWidget::mouseReleaseEvent(event);
    }

    int NuOpenGLWidget::QtKeyToKeyCode(int qtKey)
    {
        if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
        {
            return qtKey;
        }

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

    void NuOpenGLWidget::InitializeApp()
    {
        if (!m_App || !m_GLInitialized) return;
        makeCurrent();
        m_App->InitializeGraphicsForEditor();
        m_App->OnInit();
        m_App->ResizeViewport(width(), height());
        doneCurrent();
    }
} // namespace NuEditor