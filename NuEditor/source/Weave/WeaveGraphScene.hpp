#pragma once

#include <QPointF>
#include <QRectF>
#include <QColor>
#include <QString>
#include <QVector>
#include <QPainter>
#include <QObject>

#include <unordered_set>

#include <NuEngine/Weave/WeaveTypes.hpp>

class QMouseEvent;
class QKeyEvent;

namespace NuEditor::Weave
{
    class WeaveGraphView;

    using WeaveNodeKind = NuEngine::Weave::NodeKind;

    struct WeavePin
    {
        QString Name;
        bool IsOutput = false;
        QColor Color = QColor(0xA0, 0xA0, 0xA0);
    };

    struct WeaveNode
    {
        int Id = 0;
        WeaveNodeKind Kind = WeaveNodeKind::Unknown;
        QString Title;
        QString Subtitle;
        QVector<QString> PinDefaultValues;
        QPointF Position;
        QSizeF Size = { 160, 60 };
        QColor HeaderColor;
        QVector<WeavePin> Pins;

        mutable QVector<QPointF> PinPositions;

        [[nodiscard]] float GetDefaultFloat(int pinIdx) const noexcept
        {
            if (pinIdx < PinDefaultValues.size() && !PinDefaultValues[pinIdx].isEmpty())
            {
                bool ok = false;
                float v = PinDefaultValues[pinIdx].toFloat(&ok);
                return ok ? v : 0.0f;
            }
            return 0.0f;
        }
    };

    struct WeaveNodeDef
    {
        WeaveNodeKind Kind;
        QString Title;
        QColor HeaderColor;
        QVector<WeavePin> Pins;
        QString Category;

        QVector<QString> DefaultValues;
    };

    struct WeaveConnection
    {
        int FromNodeId = -1;
        int FromPinIdx = -1;
        int ToNodeId = -1;
        int ToPinIdx = -1;
    };

    class WeaveNodeRegistry
    {
    public:
        [[nodiscard]] static const WeaveNodeRegistry& Get();

        [[nodiscard]] const WeaveNodeDef* Find(WeaveNodeKind kind) const noexcept;

        [[nodiscard]] const QVector<WeaveNodeDef>& All() const noexcept { return m_Defs; }

    private:
        WeaveNodeRegistry();

        QVector<WeaveNodeDef> m_Defs;
    };

    struct InlineEditTarget
    {
        int NodeId = -1;
        int PinIdx = -1;
        QRectF ScreenRect;
    };

    class WeaveGraphScene : public QObject
    {
        Q_OBJECT
    public:
        explicit WeaveGraphScene(QObject* parent = nullptr);

        [[nodiscard]] const QVector<WeaveNode>& GetNodes() const { return m_Nodes; }
        [[nodiscard]] const QVector<WeaveConnection>& GetConnections() const { return m_Connections; }
        [[nodiscard]] QRectF GetBoundingRect() const;

        [[nodiscard]] QVector<int> GetSelection() const
        {
            return QVector<int>(m_SelectedNodeIds.begin(), m_SelectedNodeIds.end());
        }

        void SetSelection(const QVector<int>& ids)
        {
            m_SelectedNodeIds.clear();
            for (int id : ids)
                m_SelectedNodeIds.insert(id);
            emit SceneChanged();
        }

        void AddToSelection(int id);
        void ClearSelection();
        [[nodiscard]] bool IsSelected(int id) const;

        [[nodiscard]] int SpawnNode(WeaveNodeKind kind, QPointF position = {});
        int AddNode(WeaveNode node);
        void RemoveNode(int id);
        bool AddConnection(WeaveConnection conn);

        int AddNodeWithId(WeaveNode node);
        void RemoveConnection(int fromNodeId, int fromPinIdx, int toNodeId, int toPinIdx);

        [[nodiscard]] WeaveNode* FindNode(int id);
        [[nodiscard]] const WeaveNode* FindNode(int id) const;

        void Draw(QPainter& p) const;

        [[nodiscard]] bool HasActiveEdit() const { return m_EditTarget.NodeId != -1; }
        [[nodiscard]] const InlineEditTarget& GetEditTarget() const { return m_EditTarget; }
        void CommitEdit(const QString& value);
        void CancelEdit();

        void HandleMousePress(QMouseEvent* e, WeaveGraphView* view);
        void HandleMouseMove(QMouseEvent* e, WeaveGraphView* view);
        void HandleMouseRelease(QMouseEvent* e, WeaveGraphView* view);
        void HandleKeyPress(QKeyEvent* e, WeaveGraphView* view);
        void HandleDoubleClick(QMouseEvent* e, WeaveGraphView* view);

    signals:
        void SceneChanged();

        void InlineEditRequested(const InlineEditTarget& target, const QString& currentValue);
        void InlineEditFinished();

        void ConnectionRequested(WeaveConnection conn);
        void NodeRemoveRequested(int nodeId);
        void DisconnectRequested(WeaveConnection conn);
        void SpawnNodeRequested(WeaveNodeKind kind, QPointF pos);

    private:
        void DrawNode(QPainter& p, const WeaveNode& node) const;
        void DrawConnection(QPainter& p, const WeaveConnection& conn) const;
        void DrawPendingWire(QPainter& p) const;
        void UpdatePinPositions(const WeaveNode& node) const;

        [[nodiscard]] QPointF PinScreenPos(const WeaveNode& node, int pinIdx) const;
        [[nodiscard]] int NodeIndexAt(const QPointF& graphPos) const;
        [[nodiscard]] int PinAt(const QPointF& graphPos, int& outPinIdx) const;

        [[nodiscard]] bool HasPath(int fromId, int toId) const;

        QVector<WeaveNode>       m_Nodes;
        QVector<WeaveConnection> m_Connections;
        int m_NextId = 1;

        int m_DragNodeId = -1;
        std::unordered_set<int> m_SelectedNodeIds;
        QPointF m_LastDragMousePos;

        bool m_IsBoxSelecting = false;
        QPointF m_BoxSelectStart;
        QRectF m_SelectionBox;

        bool m_DrawingWire = false;
        int m_WireFromNode = -1;
        int m_WireFromPin = -1;
        QPointF m_WireEndPos;

        InlineEditTarget m_EditTarget;
    };
} // namespace NuEditor::Weave