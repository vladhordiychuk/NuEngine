#pragma once

#include <QWidget>
#include <QLabel>
#include <QMenuBar>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMouseEvent>

namespace NuEditor
{
    class CustomTitleBar : public QWidget
    {
        Q_OBJECT

    public:
        explicit CustomTitleBar(QWidget* parent = nullptr);

        QMenuBar* MenuBar() const { return m_MenuBar; }
        void ForceMaxIconState(bool isMaximized);

    signals:
        void MinimizeRequested();
        void MaximizeRequested();
        void CloseRequested();

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;

    private:
        void SetupUI();

    private:
        QLabel* m_LogoLabel;
        QMenuBar* m_MenuBar;
        QToolButton* m_MinBtn;
        QToolButton* m_MaxBtn;
        QToolButton* m_CloseBtn;

        bool m_Dragging = false;
        QPoint m_DragOffset;
    };
}