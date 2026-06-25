#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QScreen>
#include <QGuiApplication>

namespace NuEditor
{
    class ResizableWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit ResizableWindow(QWidget* parent = nullptr);

        void ToggleCustomMaximize();
        bool IsCustomMaximized() const { return m_IsCustomMaximized; }

        QRect RestoreFromDrag();

    signals:
        void customMaximizeStateChanged(bool isMaximized);

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        bool eventFilter(QObject* obj, QEvent* e) override;
        bool event(QEvent* e) override;
        void showEvent(QShowEvent* event) override;

        void installMouseTracking(QWidget* w);
        virtual void OnResizeAnimationStart() {}
        virtual void OnResizeAnimationEnd() {}

    private:
        enum class Edge 
        { 
            None, 
            Left, 
            Right, 
            Top, 
            Bottom,
            TopLeft, 
            TopRight, 
            BottomLeft, 
            BottomRight 
        };

        Edge HitTest(const QPoint& globalPos) const;
        void UpdateCursor(Edge edge);
        void AnimateToGeometry(const QRect& target);

    private:
        static constexpr int BORDER = 6;
        bool m_Resizing = false;
        Edge m_ResizeEdge = Edge::None;
        QPoint m_ResizeStart;
        QRect m_StartGeom;

        QPropertyAnimation* m_AnimGeom = nullptr;

        bool m_IsCustomMaximized = false;
        QRect m_RestoredGeometry;
        bool m_GeometryInitialized = false;
    };
}