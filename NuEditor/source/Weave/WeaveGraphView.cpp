#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <cmath>

#include <Weave/WeaveGraphView.hpp>
#include <Weave/WeaveGraphScene.hpp>
#include <Core/ThemeManager.hpp>

namespace NuEditor::Weave
{
    WeaveGraphView::WeaveGraphView(QWidget* parent)
        : QWidget(parent)
    {
        setObjectName("weaveGraphView");
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
        setAttribute(Qt::WA_OpaquePaintEvent, true);

        m_Pan = QPointF(0, 0);

        m_InlineEditor = new QLineEdit(this);
        m_InlineEditor->setObjectName("weaveInlineEditor");
        m_InlineEditor->hide();
        m_InlineEditor->setFrame(false);
        m_InlineEditor->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        UpdateInlineEditorStyle();

        connect(m_InlineEditor, &QLineEdit::editingFinished,
            this, &WeaveGraphView::OnEditorEditingFinished);

        connect(&Core::ThemeManager::Get(), &Core::ThemeManager::ThemeChanged,
            this, [this]() {
                UpdateInlineEditorStyle();
                update();
            });
    }

    void WeaveGraphView::UpdateInlineEditorStyle()
    {
        const auto& pal = Core::ThemeManager::Get().Palette();
        m_InlineEditor->setStyleSheet(
            QString(
                "QLineEdit#weaveInlineEditor {"
                "  background: %1;"
                "  color: %2;"
                "  border: 1px solid %3;"
                "  border-radius: 2px;"
                "  padding: 0 3px;"
                "  font-size: 8pt;"
                "  font-family: 'Segoe UI';"
                "}"
            )
            .arg(pal.Surface.name(),
                pal.Text.name(),
                pal.Accent.name())
        );
    }

    void WeaveGraphView::SetScene(WeaveGraphScene* scene)
    {
        if (m_Scene)
            disconnect(m_Scene, nullptr, this, nullptr);

        m_Scene = scene;

        if (m_Scene)
        {
            connect(m_Scene, &WeaveGraphScene::InlineEditRequested,
                this, &WeaveGraphView::OnInlineEditRequested);
            connect(m_Scene, &WeaveGraphScene::InlineEditFinished,
                this, &WeaveGraphView::OnInlineEditFinished);
        }

        update();
    }

    QPointF WeaveGraphView::ScreenToGraph(const QPointF& s) const
    {
        QPointF centre(width() * 0.5, height() * 0.5);
        return (s - centre) / m_Zoom - m_Pan;
    }

    QPointF WeaveGraphView::GraphToScreen(const QPointF& g) const
    {
        QPointF centre(width() * 0.5, height() * 0.5);
        return (g + m_Pan) * m_Zoom + centre;
    }

    void WeaveGraphView::paintEvent(QPaintEvent*)
    {
        const auto& pal = Core::ThemeManager::Get().Palette();

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, false);

        if (m_IsFrozen && !m_FrozenSnapshot.isNull())
        {
            p.fillRect(rect(), pal.Background);
            int dx = (width() - m_FrozenSize.width()) / 2;
            int dy = (height() - m_FrozenSize.height()) / 2;
            p.drawPixmap(dx, dy, m_FrozenSnapshot);
            return;
        }

        p.fillRect(rect(), pal.Background);
        DrawGrid(p);

        if (m_Scene)
        {
            p.save();
            QPointF centre(width() * 0.5, height() * 0.5);
            p.translate(centre);
            p.scale(m_Zoom, m_Zoom);
            p.translate(m_Pan);
            p.setRenderHint(QPainter::Antialiasing, true);
            m_Scene->Draw(p);
            p.restore();
        }

        if (m_ShowMinimap)
            DrawMinimap(p);
    }

    void WeaveGraphView::DrawGrid(QPainter& p)
    {
        const auto& pal = Core::ThemeManager::Get().Palette();
        const float zoom = m_Zoom;
        const QPointF centre(width() * 0.5f, height() * 0.5f);

        QPointF topLeft = ScreenToGraph(QPointF(0, 0));
        QPointF bottomRight = ScreenToGraph(QPointF(width(), height()));

        {
            QPen pen(pal.GridLine, 1.0f / zoom);
            p.setPen(pen);

            int x0 = (int)std::floor(topLeft.x() / k_GridLarge) * k_GridLarge;
            int y0 = (int)std::floor(topLeft.y() / k_GridLarge) * k_GridLarge;
            int x1 = (int)std::ceil(bottomRight.x() / k_GridLarge) * k_GridLarge;
            int y1 = (int)std::ceil(bottomRight.y() / k_GridLarge) * k_GridLarge;

            p.save();
            p.translate(centre);
            p.scale(zoom, zoom);
            p.translate(m_Pan);

            for (int gx = x0; gx <= x1; gx += k_GridLarge)
                p.drawLine(QPointF(gx, topLeft.y()), QPointF(gx, bottomRight.y()));
            for (int gy = y0; gy <= y1; gy += k_GridLarge)
                p.drawLine(QPointF(topLeft.x(), gy), QPointF(bottomRight.x(), gy));

            p.restore();
        }

        if (zoom >= 0.35f)
        {
            const float dotRadius = qMax(1.0f, 1.2f * zoom);
            p.setPen(Qt::NoPen);
            p.setBrush(pal.GridDot);

            int x0 = (int)std::floor(topLeft.x() / k_GridSmall) * k_GridSmall;
            int y0 = (int)std::floor(topLeft.y() / k_GridSmall) * k_GridSmall;
            int x1 = (int)std::ceil(bottomRight.x() / k_GridSmall) * k_GridSmall;
            int y1 = (int)std::ceil(bottomRight.y() / k_GridSmall) * k_GridSmall;

            for (int gx = x0; gx <= x1; gx += k_GridSmall)
                for (int gy = y0; gy <= y1; gy += k_GridSmall)
                {
                    QPointF sp = GraphToScreen(QPointF(gx, gy));
                    p.drawEllipse(sp, dotRadius, dotRadius);
                }
        }
    }

    void WeaveGraphView::DrawMinimap(QPainter& p)
    {
        const auto& pal = Core::ThemeManager::Get().Palette();

        const int W = k_MinimapW;
        const int H = k_MinimapH;
        const int M = k_MinimapMargin;

        QRect mmRect(width() - W - M, height() - H - M, W, H);

        p.fillRect(mmRect, pal.MinimapBg);
        p.setPen(QPen(pal.MinimapBorder, 1));
        p.drawRect(mmRect);

        p.setPen(pal.TextMuted);
        p.setFont(QFont("Segoe UI", 7));
        p.drawText(mmRect.adjusted(4, 2, 0, 0),
            Qt::AlignTop | Qt::AlignLeft, "MINIMAP");

        const float worldW = 4096.0f;
        const float worldH = 4096.0f * (float(H) / float(W));

        auto graphToMinimap = [&](QPointF gp) -> QPointF {
            float nx = (gp.x() + worldW * 0.5f) / worldW;
            float ny = (gp.y() + worldH * 0.5f) / worldH;
            return QPointF(mmRect.left() + nx * mmRect.width(),
                mmRect.top() + ny * mmRect.height());
            };

        QPointF visTopLeft = ScreenToGraph(QPointF(0, 0));
        QPointF visBottomRight = ScreenToGraph(QPointF(width(), height()));

        QRectF viewportRect(graphToMinimap(visTopLeft),
            graphToMinimap(visBottomRight));
        viewportRect = viewportRect.intersected(mmRect);

        p.fillRect(viewportRect, pal.MinimapView);
        p.setPen(QPen(pal.MinimapBorder, 1));
        p.drawRect(viewportRect);

        if (m_Scene)
        {
            p.save();
            p.setClipRect(mmRect);
            for (const auto& node : m_Scene->GetNodes())
            {
                QPointF nmp = graphToMinimap(node.Position);
                p.fillRect(QRectF(nmp, QSizeF(6, 4)), node.HeaderColor);
            }
            p.restore();
        }
    }

    void WeaveGraphView::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::MiddleButton ||
            (event->button() == Qt::LeftButton &&
                event->modifiers() & Qt::AltModifier))
        {
            m_Panning = true;
            m_PanLastMouse = event->position();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
            return;
        }

        if (m_InlineEditor && m_InlineEditor->isVisible())
        {
            if (m_InlineEditor->geometry().contains(event->pos()))
            {
                QWidget::mousePressEvent(event);
                return;
            }
            OnEditorEditingFinished();
        }

        if (m_Scene)
            m_Scene->HandleMousePress(event, this);

        QWidget::mousePressEvent(event);
    }

    void WeaveGraphView::mouseMoveEvent(QMouseEvent* event)
    {
        if (m_Panning)
        {
            QPointF delta = event->position() - m_PanLastMouse;
            m_Pan += delta / m_Zoom;
            m_PanLastMouse = event->position();
            update();
            event->accept();
            return;
        }

        if (m_Scene)
            m_Scene->HandleMouseMove(event, this);

        QWidget::mouseMoveEvent(event);
    }

    void WeaveGraphView::mouseReleaseEvent(QMouseEvent* event)
    {
        if (m_Panning &&
            (event->button() == Qt::MiddleButton ||
                event->button() == Qt::LeftButton))
        {
            m_Panning = false;
            unsetCursor();
            event->accept();
            return;
        }

        if (m_Scene)
            m_Scene->HandleMouseRelease(event, this);

        QWidget::mouseReleaseEvent(event);
    }

    void WeaveGraphView::wheelEvent(QWheelEvent* event)
    {
        QPointF mouseScreen = event->position();
        QPointF mouseGraphBefore = ScreenToGraph(mouseScreen);

        float delta = event->angleDelta().y() > 0 ? k_ZoomStep : -k_ZoomStep;
        m_Zoom = qBound(k_ZoomMin, m_Zoom + delta * m_Zoom, k_ZoomMax);

        QPointF mouseGraphAfter = ScreenToGraph(mouseScreen);
        m_Pan += (mouseGraphAfter - mouseGraphBefore);

        update();
        event->accept();
    }

    void WeaveGraphView::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_F)
        {
            ZoomToFit();
            event->accept();
            return;
        }

        if (m_Scene)
            m_Scene->HandleKeyPress(event, this);

        QWidget::keyPressEvent(event);
    }

    void WeaveGraphView::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (m_Scene)
            m_Scene->HandleDoubleClick(event, this);
        QWidget::mouseDoubleClickEvent(event);
    }

    void WeaveGraphView::resizeEvent(QResizeEvent* event)
    {
        QWidget::resizeEvent(event);
        update();
    }

    void WeaveGraphView::ZoomToFit()
    {
        if (!m_Scene || m_Scene->GetNodes().empty())
        {
            m_Zoom = 1.0f;
            m_Pan = QPointF(0, 0);
            update();
            return;
        }

        QRectF bounds = m_Scene->GetBoundingRect();
        if (bounds.isEmpty())
        {
            m_Pan = QPointF(0, 0);
            update();
            return;
        }

        const float padding = 80.0f;
        float zoomX = (width() - padding * 2) / bounds.width();
        float zoomY = (height() - padding * 2) / bounds.height();
        m_Zoom = qBound(k_ZoomMin, qMin(zoomX, zoomY), k_ZoomMax);
        m_Pan = -bounds.center();
        update();
    }

    void WeaveGraphView::OnInlineEditRequested(const InlineEditTarget& target,
        const QString& currentValue)
    {
        Q_UNUSED(target);
        if (!m_InlineEditor) return;

        QRect editorRect(
            qRound(target.ScreenRect.left()),
            qRound(target.ScreenRect.top()),
            qRound(target.ScreenRect.width()),
            qRound(target.ScreenRect.height()));

        m_InlineEditor->setGeometry(editorRect);
        m_InlineEditor->setText(currentValue);
        m_InlineEditor->show();
        m_InlineEditor->setFocus();
        m_InlineEditor->selectAll();
        update();
    }

    void WeaveGraphView::OnInlineEditFinished()
    {
        if (m_InlineEditor)
            m_InlineEditor->hide();
        update();
    }

    void WeaveGraphView::OnEditorEditingFinished()
    {
        if (!m_Scene || !m_InlineEditor) return;

        QString value = m_InlineEditor->text().trimmed();
        bool ok = false;
        value.toFloat(&ok);
        if (!ok) value = "0.0";

        m_InlineEditor->hide();
        m_Scene->CommitEdit(value);
        update();
    }

    void WeaveGraphView::SetFreezeFrame(bool freeze)
    {
        if (freeze && !m_IsFrozen)
        {
            m_FrozenSnapshot = grab();
            m_FrozenSize = size();
            m_IsFrozen = true;
        }
        else if (!freeze && m_IsFrozen)
        {
            m_IsFrozen = false;
            m_FrozenSnapshot = QPixmap();
            update();
        }
    }

} // namespace NuEditor::Weave