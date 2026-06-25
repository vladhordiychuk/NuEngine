#pragma once

#include <QWidget>
#include <QPointF>
#include <QLineEdit>
#include <QPixmap>

namespace NuEditor::Weave
{

    class WeaveGraphScene;

    class WeaveGraphView : public QWidget
    {
        Q_OBJECT
    public:
        explicit WeaveGraphView(QWidget* parent = nullptr);

        void SetScene(WeaveGraphScene* scene);
        void ZoomToFit();
        void SetMinimapVisible(bool visible) { m_ShowMinimap = visible; }

        [[nodiscard]] float   GetZoom() const { return m_Zoom; }
        [[nodiscard]] QPointF GetPan()  const { return m_Pan; }

        [[nodiscard]] QPointF ScreenToGraph(const QPointF& screen) const;
        [[nodiscard]] QPointF GraphToScreen(const QPointF& graph)  const;

        void SetFreezeFrame(bool freeze);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    private slots:
        void OnInlineEditRequested(const struct InlineEditTarget& target,
            const QString& currentValue);
        void OnInlineEditFinished();
        void OnEditorEditingFinished();

    private:
        void UpdateInlineEditorStyle();
        void DrawGrid(QPainter& painter);
        void DrawMinimap(QPainter& painter);

        WeaveGraphScene* m_Scene = nullptr;

        float   m_Zoom = 1.0f;
        QPointF m_Pan = { 0.0f, 0.0f };

        static constexpr float k_ZoomMin = 0.1f;
        static constexpr float k_ZoomMax = 4.0f;
        static constexpr float k_ZoomStep = 0.12f;
        static constexpr int   k_GridSmall = 16;
        static constexpr int   k_GridLarge = 128;

        bool    m_Panning = false;
        QPointF m_PanLastMouse;

        bool m_IsFrozen = false;
        QPixmap m_FrozenSnapshot;
        QSize m_FrozenSize;

        bool m_ShowMinimap = true;
        static constexpr int k_MinimapW = 200;
        static constexpr int k_MinimapH = 130;
        static constexpr int k_MinimapMargin = 12;

        QLineEdit* m_InlineEditor = nullptr;
    };

} // namespace NuEditor::Weave