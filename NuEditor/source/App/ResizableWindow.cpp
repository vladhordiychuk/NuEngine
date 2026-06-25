#include <App/ResizableWindow.hpp>

namespace NuEditor
{
    ResizableWindow::ResizableWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setAttribute(Qt::WA_Hover, true);
        setMouseTracking(true);
    }

    ResizableWindow::Edge ResizableWindow::HitTest(const QPoint& globalPos) const
    {
        QRect r = frameGeometry();
        QPoint pos = globalPos - r.topLeft();

        int x = pos.x();
        int y = pos.y();
        int w = r.width();
        int h = r.height();

        bool onLeft = x <= BORDER;
        bool onRight = x >= w - BORDER;
        bool onTop = y <= BORDER;
        bool onBottom = y >= h - BORDER;

        if (onTop && onLeft)      return Edge::TopLeft;
        if (onTop && onRight)     return Edge::TopRight;
        if (onBottom && onLeft)   return Edge::BottomLeft;
        if (onBottom && onRight)  return Edge::BottomRight;
        if (onLeft)               return Edge::Left;
        if (onRight)              return Edge::Right;
        if (onTop)                return Edge::Top;
        if (onBottom)             return Edge::Bottom;

        return Edge::None;
    }

    void ResizableWindow::UpdateCursor(Edge edge)
    {
        switch (edge)
        {
        case Edge::Right:
        case Edge::Left:
            setCursor(Qt::SizeHorCursor);
            break;
        case Edge::Top:
        case Edge::Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        case Edge::TopLeft:
        case Edge::BottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Edge::TopRight:
        case Edge::BottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            unsetCursor();
            break;
        }
    }

    void ResizableWindow::showEvent(QShowEvent* event)
    {
        if (!m_GeometryInitialized)
        {
            m_RestoredGeometry = frameGeometry();

            if (m_RestoredGeometry.width() < minimumWidth() || m_RestoredGeometry.height() < minimumHeight())
            {
                QRect avail = screen()->availableGeometry();
                QSize defSize(1280, 720);
                m_RestoredGeometry = QRect(
                    avail.center() - QPoint(defSize.width() / 2, defSize.height() / 2),
                    defSize
                );
                setGeometry(m_RestoredGeometry);
            }
            m_GeometryInitialized = true;
        }
        QMainWindow::showEvent(event);
    }

    void ResizableWindow::ToggleCustomMaximize()
    {
        if (m_IsCustomMaximized)
        {
            m_IsCustomMaximized = false;
            emit customMaximizeStateChanged(false);
            AnimateToGeometry(m_RestoredGeometry);
        }
        else
        {
            m_RestoredGeometry = frameGeometry();
            m_IsCustomMaximized = true;
            emit customMaximizeStateChanged(true);
            AnimateToGeometry(screen()->availableGeometry());
        }
    }

    QRect ResizableWindow::RestoreFromDrag()
    {
        if (m_AnimGeom)
        {
            m_AnimGeom->stop();
            m_AnimGeom->deleteLater();
            m_AnimGeom = nullptr;
            OnResizeAnimationEnd();
        }

        m_IsCustomMaximized = false;
        emit customMaximizeStateChanged(false);

        return m_RestoredGeometry;
    }

    void ResizableWindow::mousePressEvent(QMouseEvent* event)
    {
        if (m_AnimGeom && m_AnimGeom->state() == QAbstractAnimation::Running)
        {
            event->accept();
            return;
        }

        if (event->button() == Qt::LeftButton && !m_IsCustomMaximized)
        {
            Edge edge = HitTest(event->globalPosition().toPoint());

            if (edge != Edge::None)
            {
                m_Resizing = true;
                m_ResizeEdge = edge;
                m_ResizeStart = event->globalPosition().toPoint();
                m_StartGeom = frameGeometry();
                event->accept();
                return;
            }
        }
        QMainWindow::mousePressEvent(event);
    }

    void ResizableWindow::mouseMoveEvent(QMouseEvent* event)
    {
        QPoint globalPos = event->globalPosition().toPoint();

        if (m_Resizing && !m_IsCustomMaximized)
        {
            QPoint delta = globalPos - m_ResizeStart;
            QRect  r = m_StartGeom;
            int    minW = minimumWidth() > 0 ? minimumWidth() : 400;
            int    minH = minimumHeight() > 0 ? minimumHeight() : 300;

            switch (m_ResizeEdge)
            {
            case Edge::Right:
                r.setRight(r.right() + delta.x());
                if (r.width() < minW) r.setWidth(minW);
                break;
            case Edge::Bottom:
                r.setBottom(r.bottom() + delta.y());
                if (r.height() < minH) r.setHeight(minH);
                break;
            case Edge::Left:
                r.setLeft(r.left() + delta.x());
                if (r.width() < minW) r.setLeft(r.right() - minW);
                break;
            case Edge::Top:
                r.setTop(r.top() + delta.y());
                if (r.height() < minH) r.setTop(r.bottom() - minH);
                break;
            case Edge::TopLeft:
                r.setTopLeft(r.topLeft() + delta);
                if (r.width() < minW) r.setLeft(r.right() - minW);
                if (r.height() < minH) r.setTop(r.bottom() - minH);
                break;
            case Edge::TopRight:
                r.setTopRight(r.topRight() + delta);
                if (r.width() < minW) r.setWidth(minW);
                if (r.height() < minH) r.setTop(r.bottom() - minH);
                break;
            case Edge::BottomLeft:
                r.setBottomLeft(r.bottomLeft() + delta);
                if (r.width() < minW) r.setLeft(r.right() - minW);
                if (r.height() < minH) r.setHeight(minH);
                break;
            case Edge::BottomRight:
                r.setBottomRight(r.bottomRight() + delta);
                if (r.width() < minW) r.setWidth(minW);
                if (r.height() < minH) r.setHeight(minH);
                break;
            default:
                break;
            }

            setGeometry(r);
            event->accept();
            return;
        }

        if (!m_IsCustomMaximized)
        {
            UpdateCursor(HitTest(globalPos));
        }
        else
        {
            unsetCursor();
        }

        QMainWindow::mouseMoveEvent(event);
    }

    void ResizableWindow::mouseReleaseEvent(QMouseEvent* event)
    {
        if (m_Resizing)
        {
            m_Resizing = false;
            m_ResizeEdge = Edge::None;
            unsetCursor();
            event->accept();
            return;
        }
        QMainWindow::mouseReleaseEvent(event);
    }

    bool ResizableWindow::event(QEvent* e)
    {
        if (e->type() == QEvent::Leave)
        {
            unsetCursor();
        }
        return QMainWindow::event(e);
    }

    void ResizableWindow::installMouseTracking(QWidget* w)
    {
        w->setMouseTracking(true);
        w->installEventFilter(this);
        for (QObject* child : w->children())
        {
            if (QWidget* cw = qobject_cast<QWidget*>(child))
            {
                installMouseTracking(cw);
            }
        }
    }

    bool ResizableWindow::eventFilter(QObject* obj, QEvent* e)
    {
        switch (e->type())
        {
        case QEvent::MouseMove:
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);

            if (m_IsCustomMaximized && !m_Resizing) return false;

            if (HitTest(me->globalPosition().toPoint()) != Edge::None || m_Resizing)
            {
                QMouseEvent forwarded(
                    me->type(),
                    mapFromGlobal(me->globalPosition().toPoint()),
                    me->globalPosition(),
                    me->button(),
                    me->buttons(),
                    me->modifiers()
                );
                mouseMoveEvent(&forwarded);
                return true;
            }
            return false;
        }
        case QEvent::MouseButtonPress:
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            QMouseEvent forwarded(
                me->type(),
                mapFromGlobal(me->globalPosition().toPoint()),
                me->globalPosition(),
                me->button(),
                me->buttons(),
                me->modifiers()
            );
            mousePressEvent(&forwarded);
            return forwarded.isAccepted();
        }
        case QEvent::MouseButtonRelease:
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            QMouseEvent forwarded(
                me->type(),
                mapFromGlobal(me->globalPosition().toPoint()),
                me->globalPosition(),
                me->button(),
                me->buttons(),
                me->modifiers()
            );
            mouseReleaseEvent(&forwarded);
            return forwarded.isAccepted();
        }
        default:
            break;
        }
        return QMainWindow::eventFilter(obj, e);
    }

    void ResizableWindow::AnimateToGeometry(const QRect& target)
    {
        if (m_AnimGeom)
        {
            m_AnimGeom->stop();
            m_AnimGeom->deleteLater();
            m_AnimGeom = nullptr;
        }

        OnResizeAnimationStart();

        m_AnimGeom = new QPropertyAnimation(this, "geometry", this);

        connect(m_AnimGeom, &QPropertyAnimation::finished, this, [this]() {
            m_AnimGeom->deleteLater();
            m_AnimGeom = nullptr;
            OnResizeAnimationEnd();
            });

        m_AnimGeom->setDuration(150);
        m_AnimGeom->setStartValue(frameGeometry());
        m_AnimGeom->setEndValue(target);
        m_AnimGeom->setEasingCurve(QEasingCurve::OutCubic);
        m_AnimGeom->start();
    }
} // namespace NuEditor