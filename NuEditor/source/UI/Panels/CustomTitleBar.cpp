#include <QWindow>

#include <UI/Panels/CustomTitleBar.hpp>
#include <App/ResizableWindow.hpp>

namespace NuEditor
{
    CustomTitleBar::CustomTitleBar(QWidget* parent)
        : QWidget(parent)
    {
        setFixedHeight(28);
        setObjectName("customTitleBar");
        SetupUI();
    }

    void CustomTitleBar::SetupUI()
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(6, 0, 0, 0);
        layout->setSpacing(0);

        m_LogoLabel = new QLabel("NuEditor", this);
        m_LogoLabel->setObjectName("titleBarLogo");
        m_LogoLabel->setFixedWidth(85);
        layout->addWidget(m_LogoLabel);

        m_MenuBar = new QMenuBar(this);
        m_MenuBar->setObjectName("titleBarMenuBar");
        m_MenuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        layout->addWidget(m_MenuBar);

        layout->addStretch(1);

        m_MinBtn = new QToolButton(this);
        m_MinBtn->setObjectName("titleBarMinBtn");
        m_MinBtn->setFixedSize(46, 28);
        m_MinBtn->setIcon(QIcon(":/icons/min.svg"));

        m_MaxBtn = new QToolButton(this);
        m_MaxBtn->setObjectName("titleBarMaxBtn");
        m_MaxBtn->setFixedSize(46, 28);
        m_MaxBtn->setIcon(QIcon(":/icons/square.svg"));

        m_CloseBtn = new QToolButton(this);
        m_CloseBtn->setObjectName("titleBarCloseBtn");
        m_CloseBtn->setFixedSize(46, 28);
        m_CloseBtn->setIcon(QIcon(":/icons/x.svg"));

        layout->addWidget(m_MinBtn);
        layout->addWidget(m_MaxBtn);
        layout->addWidget(m_CloseBtn);

        connect(m_MinBtn, &QToolButton::clicked, this, &CustomTitleBar::MinimizeRequested);
        connect(m_MaxBtn, &QToolButton::clicked, this, &CustomTitleBar::MaximizeRequested);
        connect(m_CloseBtn, &QToolButton::clicked, this, &CustomTitleBar::CloseRequested);
    }

    void CustomTitleBar::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            if (childAt(event->pos()) != nullptr)
            {
                QWidget::mousePressEvent(event);
                return;
            }
            m_Dragging = true;
            m_DragOffset = event->globalPosition().toPoint() - window()->frameGeometry().topLeft();
        }
        QWidget::mousePressEvent(event);
    }

    void CustomTitleBar::mouseMoveEvent(QMouseEvent* event)
    {
        if (!m_Dragging) {
            QWidget::mouseMoveEvent(event);
            return;
        }

        auto* win = qobject_cast<ResizableWindow*>(window());
        if (!win) return;

        QPoint globalPos = event->globalPosition().toPoint();

        if (win->IsCustomMaximized())
        {
            QRect oldGeom = win->frameGeometry();

            double ratio = double(globalPos.x() - oldGeom.left()) / double(oldGeom.width());

            QRect restored = win->RestoreFromDrag();

            win->setGeometry(restored);

            int newX = globalPos.x() - int(restored.width() * ratio);
            int newY = globalPos.y() - m_DragOffset.y();

            m_DragOffset.setX(int(restored.width() * ratio));

            win->move(newX, newY);
            return;
        }

        win->move(globalPos - m_DragOffset);
    }

    void CustomTitleBar::mouseReleaseEvent(QMouseEvent* event)
    {
        m_Dragging = false;
        QWidget::mouseReleaseEvent(event);
    }

    void CustomTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            emit MaximizeRequested();
        }
        QWidget::mouseDoubleClickEvent(event);
    }

    void CustomTitleBar::ForceMaxIconState(bool isMaximized)
    {
        m_MaxBtn->setIcon(isMaximized ? QIcon(":/icons/max.svg") : QIcon(":/icons/square.svg"));
    }
} // namespace NuEditor