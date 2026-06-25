#pragma once

#include <QVector>
#include <QSet>
#include <QMap>
#include <QString>
#include <memory>

#include <Weave/WeaveGraphScene.hpp>

namespace NuEditor::Weave
{
    class WeaveCommand
    {
    public:
        virtual ~WeaveCommand() = default;
        virtual void Execute(WeaveGraphScene& scene) = 0;
        virtual void Undo(WeaveGraphScene& scene) = 0;

        [[nodiscard]] virtual QString Description() const = 0;
    };

    class CmdAddNode : public WeaveCommand
    {
    public:
        explicit CmdAddNode(WeaveNode node)
            : m_Node(std::move(node)) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            m_AssignedId = scene.AddNode(m_Node);
        }

        void Undo(WeaveGraphScene& scene) override
        {
            scene.RemoveNode(m_AssignedId);
        }

        [[nodiscard]] QString Description() const override 
        { 
            return "Add Node"; 
        }

    private:
        WeaveNode m_Node;
        int m_AssignedId = -1;
    };

    class CmdRemoveNode : public WeaveCommand
    {
    public:
        explicit CmdRemoveNode(int nodeId)
            : m_NodeId(nodeId) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            const WeaveNode* node = scene.FindNode(m_NodeId);
            if (node)
            {
                m_SavedNode = *node;
            }

            for (const auto& conn : scene.GetConnections())
            {
                if (conn.FromNodeId == m_NodeId || conn.ToNodeId == m_NodeId)
                {
                    m_SavedConnections.append(conn);
                }
            }

            scene.RemoveNode(m_NodeId);
        }

        void Undo(WeaveGraphScene& scene) override
        {
            scene.AddNodeWithId(m_SavedNode);

            for (const auto& conn : m_SavedConnections)
            {
                scene.AddConnection(conn);
            }
        }

        [[nodiscard]] QString Description() const override 
        {
            return "Remove Node"; 
        }

    private:
        int m_NodeId = -1;
        WeaveNode m_SavedNode;
        QVector<WeaveConnection> m_SavedConnections;
    };

    class CmdAddConnection : public WeaveCommand
    {
    public:
        explicit CmdAddConnection(WeaveConnection connection)
            : m_Connection(connection) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            scene.AddConnection(m_Connection);
        }

        void Undo(WeaveGraphScene& scene) override
        {
            scene.RemoveConnection(m_Connection.FromNodeId, m_Connection.FromPinIdx,
                m_Connection.ToNodeId, m_Connection.ToPinIdx);
        }

        [[nodiscard]] QString Description() const override 
        { 
            return "Connect Pins"; 
        }

    private:
        WeaveConnection m_Connection;
    };

    class CmdRemoveConnection : public WeaveCommand
    {
    public:
        explicit CmdRemoveConnection(WeaveConnection connection)
            : m_Connection(connection) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            scene.RemoveConnection(m_Connection.FromNodeId, m_Connection.FromPinIdx,
                m_Connection.ToNodeId, m_Connection.ToPinIdx);
        }

        void Undo(WeaveGraphScene& scene) override
        {
            scene.AddConnection(m_Connection);
        }

        [[nodiscard]] QString Description() const override 
        { 
            return "Disconnect Pins"; 
        }

    private:
        WeaveConnection m_Connection;
    };

    class CmdMoveNode : public WeaveCommand
    {
    public:
        explicit CmdMoveNode(int nodeId, QPointF oldPos, QPointF newPos)
            : m_NodeId(nodeId), m_OldPos(oldPos), m_NewPos(newPos) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            if (WeaveNode* node = scene.FindNode(m_NodeId))
            {
                node->Position = m_NewPos;
            }
        }

        void Undo(WeaveGraphScene& scene) override
        {
            if (WeaveNode* node = scene.FindNode(m_NodeId))
            {
                node->Position = m_OldPos;
            }
        }

        [[nodiscard]] QString Description() const override 
        { 
            return "Move Node"; 
        }

    private:
        int m_NodeId;
        QPointF m_OldPos;
        QPointF m_NewPos;
    };

    class CmdSetPinValue : public WeaveCommand
    {
    public:
        explicit CmdSetPinValue(int nodeId, int pinIdx, QString oldValue, QString newValue)
            : m_NodeId(nodeId), m_PinIdx(pinIdx), m_OldValue(std::move(oldValue)), m_NewValue(std::move(newValue)) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            Apply(scene, m_NewValue);
        }

        void Undo(WeaveGraphScene& scene) override
        {
            Apply(scene, m_OldValue);
        }

        [[nodiscard]] QString Description() const override { return "Edit Pin Value"; }

    private:
        void Apply(WeaveGraphScene& scene, const QString& value)
        {
            if (WeaveNode* node = scene.FindNode(m_NodeId))
            {
                if (m_PinIdx < node->PinDefaultValues.size())
                {
                    node->PinDefaultValues[m_PinIdx] = value;

                    if (node->Kind == WeaveNodeKind::Const_Float && m_PinIdx == 0)
                    {
                        node->Subtitle = value;
                    }
                }
            }
        }

        int m_NodeId, m_PinIdx;
        QString m_OldValue, m_NewValue;
    };

    class CmdPasteNodes : public WeaveCommand
    {
    public:
        explicit CmdPasteNodes(QVector<WeaveNode> nodes, QVector<WeaveConnection> internalConns, QPointF offset)
            : m_Nodes(std::move(nodes)), m_InternalConns(std::move(internalConns)), m_Offset(offset) 
        {
        }

        void Execute(WeaveGraphScene& scene) override
        {
            m_NewIds.clear();
            QMap<int, int> remap;

            for (auto node : m_Nodes)
            {
                int oldId = node.Id;
                node.Position += m_Offset;
                int newId = scene.AddNode(node);
                remap[oldId] = newId;
                m_NewIds.append(newId);
            }

            for (auto connection : m_InternalConns)
            {
                if (!remap.contains(connection.FromNodeId) || !remap.contains(connection.ToNodeId))
                {
                    continue;
                }

                connection.FromNodeId = remap[connection.FromNodeId];
                connection.ToNodeId = remap[connection.ToNodeId];

                scene.AddConnection(connection);
            }
        }

        void Undo(WeaveGraphScene& scene) override
        {
            for (int id : m_NewIds)
            {
                scene.RemoveNode(id);
            }
        }

        [[nodiscard]] QString Description() const override 
        { 
            return "Paste Nodes"; 
        }

        [[nodiscard]] const QVector<int>& NewIds() const
        {
            return m_NewIds;
        }

    private:
        QVector<WeaveNode> m_Nodes;
        QVector<WeaveConnection> m_InternalConns;
        QPointF m_Offset;
        QVector<int> m_NewIds;
    };

    class WeaveCommandStack : public QObject
    {
        Q_OBJECT
    public:
        explicit WeaveCommandStack(WeaveGraphScene* scene, QObject* parent = nullptr)
            : QObject(parent), m_Scene(scene) 
        {
        }

        void Push(std::unique_ptr<WeaveCommand> cmd)
        {
            cmd->Execute(*m_Scene);

            m_RedoStack.clear();
            m_UndoStack.push_back(std::move(cmd));

            if (m_UndoStack.size() > k_MaxHistory)
            {
                m_UndoStack.erase(m_UndoStack.begin());
            }

            emit HistoryChanged();
        }

        void Undo()
        {
            if (m_UndoStack.empty())
            {
                return;
            }

            auto& cmd = m_UndoStack.back();
            cmd->Undo(*m_Scene);

            m_RedoStack.push_back(std::move(cmd));
            m_UndoStack.pop_back();

            emit HistoryChanged();
        }

        void Redo()
        {
            if (m_RedoStack.empty())
            {
                return;
            }

            auto& cmd = m_RedoStack.back();
            cmd->Execute(*m_Scene);

            m_UndoStack.push_back(std::move(cmd));
            m_RedoStack.pop_back();

            emit HistoryChanged();
        }

        [[nodiscard]] bool CanUndo() const 
        { 
            return !m_UndoStack.empty(); 
        }

        [[nodiscard]] bool CanRedo() const 
        { 
            return !m_RedoStack.empty(); 
        }

        [[nodiscard]] QString UndoDescription() const
        {
            return m_UndoStack.empty() ? "" : m_UndoStack.back()->Description();
        }

        [[nodiscard]] QString RedoDescription() const
        {
            return m_RedoStack.empty() ? "" : m_RedoStack.back()->Description();
        }

        void Clear()
        {
            m_UndoStack.clear();
            m_RedoStack.clear();
            emit HistoryChanged();
        }

    signals:
        void HistoryChanged();

    private:
        static constexpr size_t k_MaxHistory = 100;

        WeaveGraphScene* m_Scene;
        std::vector<std::unique_ptr<WeaveCommand>> m_UndoStack;
        std::vector<std::unique_ptr<WeaveCommand>> m_RedoStack;
    };

    class WeaveClipboard
    {
    public:
        [[nodiscard]] static WeaveClipboard& Get()
        {
            static WeaveClipboard instance;
            return instance;
        }

        void Copy(const WeaveGraphScene& scene, const QVector<int>& selectedIds)
        {
            m_Nodes.clear();
            m_InternalConns.clear();

            for (int id : selectedIds)
            {
                const WeaveNode* node = scene.FindNode(id);
                if (node)
                {
                    m_Nodes.append(*node);
                }
            }

            QSet<int> idSet(selectedIds.begin(), selectedIds.end());
            for (const auto& connection : scene.GetConnections())
            {
                if (idSet.contains(connection.FromNodeId) && idSet.contains(connection.ToNodeId))
                {
                    m_InternalConns.append(connection);
                }
            }
        }

        [[nodiscard]] bool HasData() const
        {
            return !m_Nodes.isEmpty();
        }

        [[nodiscard]] std::unique_ptr<CmdPasteNodes> MakePasteCommand(QPointF offset) const
        {
            return std::make_unique<CmdPasteNodes>(m_Nodes, m_InternalConns, offset);
        }

    private:
        WeaveClipboard() = default;

        QVector<WeaveNode> m_Nodes;
        QVector<WeaveConnection> m_InternalConns;
    };
}