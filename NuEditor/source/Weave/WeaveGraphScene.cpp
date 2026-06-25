#include <Weave/WeaveGraphScene.hpp>
#include <Weave/WeaveGraphView.hpp>
#include <Core/ThemeManager.hpp>

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

#include <cmath>
#include <algorithm>
#include <queue>
#include <unordered_map>

namespace NuEditor::Weave
{
    static constexpr float k_NodeRadius = 6.0f;
    static constexpr float k_HeaderH = 28.0f;
    static constexpr float k_PinRow = 22.0f;
    static constexpr float k_PinRadius = 6.0f;
    static constexpr float k_PinPadX = 14.0f;
    static constexpr float k_ValueBoxH = 18.0f;
    static constexpr float k_ValueBoxW = 60.0f;

    const WeaveNodeRegistry& WeaveNodeRegistry::Get()
    {
        static WeaveNodeRegistry instance;
        return instance;
    }

    const WeaveNodeDef* WeaveNodeRegistry::Find(WeaveNodeKind kind) const noexcept
    {
        for (const auto& def : m_Defs)
            if (def.Kind == kind) return &def;
        return nullptr;
    }

    WeaveNodeRegistry::WeaveNodeRegistry()
    {
        m_Defs.push_back({ WeaveNodeKind::Event_OnUpdate, "Event OnUpdate",
            QColor(0x96, 0x28, 0x28),
            {{"►", true, QColor(0xFF,0xFF,0xFF)},
             {"DeltaTime", true, QColor(0x22,0xCC,0x88)}},
            "Events", {} });

        m_Defs.push_back({ WeaveNodeKind::Event_OnCollide, "Event OnCollide",
            QColor(0x96, 0x28, 0x28),
            {{"►", true, QColor(0xFF,0xFF,0xFF)},
             {"Other", true, QColor(0xCC,0x88,0x22)}},
            "Events", {} });

        m_Defs.push_back({ WeaveNodeKind::Const_Float, "Float",
            QColor(0x28, 0x64, 0x28),
            {{"Value", true, QColor(0x22,0xCC,0x88)}},
            "Constants", {"0.0"} });

        m_Defs.push_back({ WeaveNodeKind::Const_Int, "Int",
            QColor(0x28, 0x50, 0x64),
            {{"Value", true, QColor(0x22,0x88,0xCC)}},
            "Constants", {"0"} });

        auto addMath = [&](WeaveNodeKind k, const char* name,
            const char* defA = "0.0", const char* defB = "0.0") {
                m_Defs.push_back({ k, name, QColor(0x28, 0x28, 0x78),
                    {{"A", false, QColor(0x22,0xCC,0x88)},
                     {"B", false, QColor(0x22,0xCC,0x88)},
                     {"Result", true, QColor(0x22,0xCC,0x88)}},
                    "Math", {defA, defB} });
            };
        addMath(WeaveNodeKind::Math_Add, "Add");
        addMath(WeaveNodeKind::Math_Sub, "Subtract");
        addMath(WeaveNodeKind::Math_Mul, "Multiply", "0.0", "1.0");
        addMath(WeaveNodeKind::Math_Div, "Divide", "0.0", "1.0");

        m_Defs.push_back({ WeaveNodeKind::Math_Neg, "Negate",
            QColor(0x28, 0x28, 0x78),
            {{"A", false, QColor(0x22,0xCC,0x88)},
             {"Result", true, QColor(0x22,0xCC,0x88)}},
            "Math", {"0.0"} });

        auto addCmp = [&](WeaveNodeKind k, const char* name) {
            m_Defs.push_back({ k, name, QColor(0x50, 0x28, 0x78),
                {{"A", false, QColor(0x22,0xCC,0x88)},
                 {"B", false, QColor(0x22,0xCC,0x88)},
                 {"Result", true, QColor(0xCC,0x44,0x44)}},
                "Compare", {"0.0", "0.0"} });
            };
        addCmp(WeaveNodeKind::Cmp_EQ, "Equal");
        addCmp(WeaveNodeKind::Cmp_LT, "Less Than");
        addCmp(WeaveNodeKind::Cmp_GT, "Greater Than");
        addCmp(WeaveNodeKind::Cmp_LE, "Less Equal");
        addCmp(WeaveNodeKind::Cmp_GE, "Greater Equal");

        m_Defs.push_back({ WeaveNodeKind::Flow_Branch, "Branch",
            QColor(0x78, 0x50, 0x14),
            {{"Condition", false, QColor(0xCC,0x44,0x44)},
             {"True ►",   true,  QColor(0xFF,0xFF,0xFF)},
             {"False ►",  true,  QColor(0xFF,0xFF,0xFF)}},
            "Flow", {} });

        m_Defs.push_back({ WeaveNodeKind::Native_GetDeltaTime, "Get DeltaTime",
            QColor(0x1A, 0x3A, 0x5C),
            {{"►", false, QColor(0xFF,0xFF,0xFF)},
             {"dt", true, QColor(0x22,0xCC,0x88)}},
            "Native", {} });

        m_Defs.push_back({ WeaveNodeKind::Native_SetVelZ, "Set Velocity Z",
            QColor(0x12, 0x50, 0x18),
            {{"►", false, QColor(0xFF,0xFF,0xFF)},
             {"Speed", false, QColor(0x22,0xCC,0x88)}},
            "Native", {"0.0"} });

        m_Defs.push_back({ WeaveNodeKind::Native_FindPlayer, "Find Player",
            QColor(0x1A, 0x3A, 0x5C),
            {{"PlayerID", true, QColor(0xCC,0x88,0x22)}},
            "Native", {} });

        m_Defs.push_back({ WeaveNodeKind::Native_DistanceTo, "Distance To",
            QColor(0x1A, 0x3A, 0x5C),
            {{"A", false, QColor(0xCC,0x88,0x22)},
             {"B", false, QColor(0xCC,0x88,0x22)},
             {"Dist", true, QColor(0x22,0xCC,0x88)}},
            "Native", {} });
    }

    WeaveGraphScene::WeaveGraphScene(QObject* parent)
        : QObject(parent)
    {
    }

    void WeaveGraphScene::Draw(QPainter& p) const
    {
        const auto& pal = Core::ThemeManager::Get().Palette();

        for (const auto& node : m_Nodes)
            UpdatePinPositions(node);

        for (const auto& conn : m_Connections)
            DrawConnection(p, conn);

        if (m_DrawingWire)
            DrawPendingWire(p);

        for (const auto& node : m_Nodes)
            DrawNode(p, node);

        if (m_IsBoxSelecting)
        {
            p.setPen(QPen(pal.Accent, 1.0f, Qt::DashLine));
            p.setBrush(QColor(pal.Accent.red(),
                pal.Accent.green(),
                pal.Accent.blue(), 30));
            p.drawRect(m_SelectionBox);
        }
    }

    void WeaveGraphScene::DrawNode(QPainter& p, const WeaveNode& node) const
    {
        const auto& pal = Core::ThemeManager::Get().Palette();

        QRectF rect(node.Position, node.Size);
        QRectF headerRect(node.Position, QSizeF(node.Size.width(), k_HeaderH));
        const bool selected = (m_SelectedNodeIds.count(node.Id) > 0);

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 80));
        p.drawRoundedRect(rect.adjusted(3, 4, 3, 4), k_NodeRadius, k_NodeRadius);

        p.setBrush(pal.NodeBg);
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(rect, k_NodeRadius, k_NodeRadius);

        {
            QPainterPath hp;
            hp.setFillRule(Qt::WindingFill);
            hp.addRoundedRect(headerRect, k_NodeRadius, k_NodeRadius);
            hp.addRect(headerRect.adjusted(0, k_NodeRadius, 0, 0));
            p.setBrush(node.HeaderColor);
            p.setPen(Qt::NoPen);
            p.drawPath(hp);
        }

        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(selected ? pal.NodeBorderSelected : pal.NodeBorder,
            selected ? 2.0f : 1.0f));
        p.drawRoundedRect(rect, k_NodeRadius, k_NodeRadius);

        p.setPen(pal.Text);
        p.setFont(QFont("Segoe UI", 9, QFont::DemiBold));
        p.drawText(headerRect.adjusted(10, 0, -6, 0),
            Qt::AlignVCenter | Qt::AlignLeft, node.Title);

        node.PinPositions.resize(node.Pins.size());
        float pinY = node.Position.y() + k_HeaderH + k_PinRow * 0.5f + 4.0f;
        p.setFont(QFont("Segoe UI", 8));

        for (int i = 0; i < node.Pins.size(); ++i)
        {
            const WeavePin& pin = node.Pins[i];
            float pinX = pin.IsOutput
                ? node.Position.x() + node.Size.width() - k_PinPadX
                : node.Position.x() + k_PinPadX;

            QPointF pinPos(pinX, pinY);
            node.PinPositions[i] = pinPos;

            p.setPen(QPen(pin.Color.lighter(140), 1.5f));
            p.setBrush(pal.PinBg);
            p.drawEllipse(pinPos, k_PinRadius, k_PinRadius);

            const bool isConstPin =
                (node.Kind == WeaveNodeKind::Const_Float ||
                    node.Kind == WeaveNodeKind::Const_Int) && i == 0;

            if (!isConstPin)
            {
                p.setPen(pal.Text);
                if (pin.IsOutput)
                {
                    QRectF lr(node.Position.x() + node.Size.width() * 0.5f,
                        pinY - 9,
                        node.Size.width() * 0.5f - k_PinPadX - k_PinRadius - 4,
                        18);
                    p.drawText(lr, Qt::AlignVCenter | Qt::AlignRight, pin.Name);
                }
                else
                {
                    QRectF lr(node.Position.x() + k_PinPadX + k_PinRadius + 4,
                        pinY - 9,
                        node.Size.width() * 0.5f - k_PinPadX - k_PinRadius - 4,
                        18);
                    p.drawText(lr, Qt::AlignVCenter | Qt::AlignLeft, pin.Name);
                }
            }
            else
            {
                const bool isEditing =
                    (m_EditTarget.NodeId == node.Id && m_EditTarget.PinIdx == i);

                const float paddingFromPin = k_PinRadius + 6.0f;
                const float boxWidth = k_ValueBoxW + 15.0f;
                const float boxLeft = pinPos.x() - paddingFromPin - boxWidth;

                QRectF vbox(boxLeft, pinY - k_ValueBoxH * 0.5f,
                    boxWidth, k_ValueBoxH);

                p.setPen(QPen(isEditing ? pal.ValueBoxActive
                    : pal.ValueBoxBorder, 1.0f));
                p.setBrush(pal.ValueBoxBg);
                p.drawRoundedRect(vbox, 3, 3);

                QString val = (i < node.PinDefaultValues.size())
                    ? node.PinDefaultValues[i] : "0.0";
                if (val.isEmpty()) val = "0.0";

                p.setPen(isEditing ? pal.ValueBoxActive : pal.Text);
                p.drawText(vbox.adjusted(4, 0, -4, 0),
                    Qt::AlignVCenter | Qt::AlignRight, val);

                QRectF textRect(node.Position.x(), pinY - 9,
                    boxLeft - node.Position.x() - 4.0f, 18);
                p.setPen(pal.Text);
                p.drawText(textRect, Qt::AlignVCenter | Qt::AlignRight,
                    pin.Name + ":");
            }

            pinY += k_PinRow;
        }
    }

    static QPainterPath MakeBezier(const QPointF& from, const QPointF& to)
    {
        float dx = qMax(std::abs(to.x() - from.x()) * 0.6f, 60.0f);
        QPainterPath path;
        path.moveTo(from);
        path.cubicTo(from + QPointF(dx, 0), to - QPointF(dx, 0), to);
        return path;
    }

    void WeaveGraphScene::DrawConnection(QPainter& p,
        const WeaveConnection& conn) const
    {
        const WeaveNode* fn = FindNode(conn.FromNodeId);
        const WeaveNode* tn = FindNode(conn.ToNodeId);
        if (!fn || !tn) return;
        if (conn.FromPinIdx >= fn->Pins.size()) return;
        if (conn.ToPinIdx >= tn->Pins.size())  return;

        QPointF from = PinScreenPos(*fn, conn.FromPinIdx);
        QPointF to = PinScreenPos(*tn, conn.ToPinIdx);

        p.setPen(QPen(fn->Pins[conn.FromPinIdx].Color,
            2.5f, Qt::SolidLine, Qt::RoundCap));
        p.setBrush(Qt::NoBrush);
        p.drawPath(MakeBezier(from, to));
    }

    void WeaveGraphScene::DrawPendingWire(QPainter& p) const
    {
        const auto& pal = Core::ThemeManager::Get().Palette();
        const WeaveNode* fn = FindNode(m_WireFromNode);
        if (!fn) return;

        p.setPen(QPen(pal.WirePending, 2.0f, Qt::DashLine, Qt::RoundCap));
        p.setBrush(Qt::NoBrush);
        p.drawPath(MakeBezier(PinScreenPos(*fn, m_WireFromPin), m_WireEndPos));
    }

    void WeaveGraphScene::HandleDoubleClick(QMouseEvent* e, WeaveGraphView* view)
    {
        if (e->button() != Qt::LeftButton) return;

        QPointF gp = view->ScreenToGraph(e->position());
        if (NodeIndexAt(gp) != -1) return;

        const auto& pal = Core::ThemeManager::Get().Palette();

        QMenu menu(view);
        menu.setStyleSheet(
            QString(
                "QMenu { background: %1; color: %2;"
                "        border: 1px solid %3; padding: 4px; }"
                "QMenu::item { padding: 4px 24px 4px 24px; }"
                "QMenu::item:selected { background: %4; color: %5; }"
                "QMenu::separator { height: 1px; background: %3; margin: 4px 0; }"
            )
            .arg(pal.Surface.name(),
                pal.Text.name(),
                pal.Border.name(),
                pal.AccentBg.name(),
                pal.Text.name())
        );

        std::unordered_map<QString, QMenu*> categoryMenus;

        for (const auto& def : WeaveNodeRegistry::Get().All())
        {
            QString cat = def.Category.isEmpty() ? "General" : def.Category;
            if (categoryMenus.find(cat) == categoryMenus.end())
                categoryMenus[cat] = menu.addMenu(cat);

            QAction* action = categoryMenus[cat]->addAction(def.Title);
            connect(action, &QAction::triggered, view,
                [this, kind = def.Kind, gp, view]() {
                    emit SpawnNodeRequested(kind, gp);
                    view->update();
                });
        }

        menu.exec(e->globalPosition().toPoint());
        e->accept();
    }

    void WeaveGraphScene::UpdatePinPositions(const WeaveNode& node) const
    {
        node.PinPositions.resize(node.Pins.size());
        float pinY = node.Position.y() + k_HeaderH + k_PinRow * 0.5f + 4.0f;
        for (int i = 0; i < node.Pins.size(); ++i)
        {
            float pinX = node.Pins[i].IsOutput
                ? node.Position.x() + node.Size.width() - k_PinPadX
                : node.Position.x() + k_PinPadX;
            node.PinPositions[i] = QPointF(pinX, pinY);
            pinY += k_PinRow;
        }
    }

    QPointF WeaveGraphScene::PinScreenPos(const WeaveNode& node, int pinIdx) const
    {
        if (pinIdx < 0 || pinIdx >= node.Pins.size()) return {};
        if (pinIdx < node.PinPositions.size()) return node.PinPositions[pinIdx];

        float pinY = node.Position.y() + k_HeaderH
            + k_PinRow * 0.5f + 4.0f + pinIdx * k_PinRow;
        float pinX = node.Pins[pinIdx].IsOutput
            ? node.Position.x() + node.Size.width() - k_PinPadX
            : node.Position.x() + k_PinPadX;
        return QPointF(pinX, pinY);
    }

    int WeaveGraphScene::AddNodeWithId(WeaveNode node)
    {
        if (node.Id >= m_NextId) m_NextId = node.Id + 1;
        float bodyH = qMax(8.0f + node.Pins.size() * k_PinRow, 40.0f);
        node.Size = QSizeF(180, k_HeaderH + bodyH);
        while (node.PinDefaultValues.size() < node.Pins.size())
            node.PinDefaultValues.append("");
        m_Nodes.append(node);
        emit SceneChanged();
        return node.Id;
    }

    int WeaveGraphScene::AddNode(WeaveNode node)
    {
        node.Id = m_NextId++;
        float bodyH = qMax(8.0f + node.Pins.size() * k_PinRow, 40.0f);
        node.Size = QSizeF(180, k_HeaderH + bodyH);
        while (node.PinDefaultValues.size() < node.Pins.size())
            node.PinDefaultValues.append("");
        m_Nodes.append(node);
        emit SceneChanged();
        return node.Id;
    }

    int WeaveGraphScene::SpawnNode(WeaveNodeKind kind, QPointF position)
    {
        const WeaveNodeDef* def = WeaveNodeRegistry::Get().Find(kind);
        if (!def) return -1;
        WeaveNode node;
        node.Kind = kind;
        node.Title = def->Title;
        node.HeaderColor = def->HeaderColor;
        node.Pins = def->Pins;
        node.Position = position;
        node.PinDefaultValues = def->DefaultValues;
        while (node.PinDefaultValues.size() < node.Pins.size())
            node.PinDefaultValues.append("");
        return AddNode(node);
    }

    void WeaveGraphScene::RemoveNode(int id)
    {
        m_Nodes.erase(std::remove_if(m_Nodes.begin(), m_Nodes.end(),
            [id](const WeaveNode& n) { return n.Id == id; }), m_Nodes.end());
        m_Connections.erase(std::remove_if(m_Connections.begin(), m_Connections.end(),
            [id](const WeaveConnection& c) {
                return c.FromNodeId == id || c.ToNodeId == id;
            }), m_Connections.end());
        if (m_EditTarget.NodeId == id) CancelEdit();
        emit SceneChanged();
    }

    void WeaveGraphScene::RemoveConnection(int fromNodeId, int fromPinIdx,
        int toNodeId, int toPinIdx)
    {
        m_Connections.erase(std::remove_if(m_Connections.begin(), m_Connections.end(),
            [&](const WeaveConnection& c) {
                return c.FromNodeId == fromNodeId && c.FromPinIdx == fromPinIdx
                    && c.ToNodeId == toNodeId && c.ToPinIdx == toPinIdx;
            }), m_Connections.end());
        emit SceneChanged();
    }

    bool WeaveGraphScene::AddConnection(WeaveConnection conn)
    {
        if (HasPath(conn.ToNodeId, conn.FromNodeId)) return false;
        for (const auto& c : m_Connections)
            if (c.ToNodeId == conn.ToNodeId && c.ToPinIdx == conn.ToPinIdx)
                return false;
        m_Connections.append(conn);
        emit SceneChanged();
        return true;
    }

    bool WeaveGraphScene::HasPath(int fromId, int toId) const
    {
        if (fromId == toId) return true;
        std::unordered_map<int, std::vector<int>> adj;
        for (const auto& c : m_Connections)
            adj[c.FromNodeId].push_back(c.ToNodeId);
        std::queue<int> q;
        std::unordered_set<int> visited;
        q.push(fromId);
        visited.insert(fromId);
        while (!q.empty())
        {
            int cur = q.front(); q.pop();
            for (int next : adj[cur])
            {
                if (next == toId) return true;
                if (!visited.count(next)) { visited.insert(next); q.push(next); }
            }
        }
        return false;
    }

    QRectF WeaveGraphScene::GetBoundingRect() const
    {
        if (m_Nodes.isEmpty()) return {};
        QRectF r(m_Nodes[0].Position, m_Nodes[0].Size);
        for (const auto& n : m_Nodes)
            r = r.united(QRectF(n.Position, n.Size));
        return r;
    }

    void WeaveGraphScene::AddToSelection(int id)
    {
        if (!m_SelectedNodeIds.count(id)) { m_SelectedNodeIds.insert(id); emit SceneChanged(); }
    }
    void WeaveGraphScene::ClearSelection()
    {
        if (!m_SelectedNodeIds.empty()) { m_SelectedNodeIds.clear(); m_DragNodeId = -1; emit SceneChanged(); }
    }
    bool WeaveGraphScene::IsSelected(int id) const { return m_SelectedNodeIds.count(id) > 0; }

    WeaveNode* WeaveGraphScene::FindNode(int id)
    {
        for (auto& n : m_Nodes) if (n.Id == id) return &n;
        return nullptr;
    }
    const WeaveNode* WeaveGraphScene::FindNode(int id) const
    {
        for (const auto& n : m_Nodes) if (n.Id == id) return &n;
        return nullptr;
    }

    int WeaveGraphScene::NodeIndexAt(const QPointF& gp) const
    {
        for (int i = m_Nodes.size() - 1; i >= 0; --i)
            if (QRectF(m_Nodes[i].Position, m_Nodes[i].Size).contains(gp))
                return i;
        return -1;
    }

    int WeaveGraphScene::PinAt(const QPointF& gp, int& outPinIdx) const
    {
        for (const auto& node : m_Nodes)
            for (int i = 0; i < node.PinPositions.size(); ++i)
                if (QLineF(node.PinPositions[i], gp).length() <= k_PinRadius + 4.0f)
                {
                    outPinIdx = i; return node.Id;
                }
        return -1;
    }

    void WeaveGraphScene::HandleMousePress(QMouseEvent* e, WeaveGraphView* view)
    {
        QPointF gp = view->ScreenToGraph(e->position());

        if (e->button() == Qt::LeftButton)
        {
            if (e->modifiers() & Qt::AltModifier)
            {
                int pinIdx = -1;
                int pinNodeId = PinAt(gp, pinIdx);
                if (pinNodeId != -1)
                    for (const auto& conn : m_Connections)
                        if ((conn.FromNodeId == pinNodeId && conn.FromPinIdx == pinIdx) ||
                            (conn.ToNodeId == pinNodeId && conn.ToPinIdx == pinIdx))
                            emit DisconnectRequested(conn);
                view->update(); e->accept(); return;
            }

            for (auto& node : m_Nodes)
            {
                float pinY = node.Position.y() + k_HeaderH + k_PinRow * 0.5f + 4.0f;
                for (int i = 0; i < node.Pins.size(); ++i)
                {
                    if ((node.Kind == WeaveNodeKind::Const_Float ||
                        node.Kind == WeaveNodeKind::Const_Int) && i == 0)
                    {
                        float pinX = node.Position.x() + node.Size.width() - k_PinPadX;
                        float boxLeft = pinX - (k_PinRadius + 6.0f) - (k_ValueBoxW + 15.0f);
                        QRectF vbox(boxLeft, pinY - k_ValueBoxH * 0.5f,
                            k_ValueBoxW + 15.0f, k_ValueBoxH);
                        if (vbox.contains(gp))
                        {
                            m_EditTarget.NodeId = node.Id;
                            m_EditTarget.PinIdx = i;
                            m_EditTarget.ScreenRect = QRectF(
                                view->GraphToScreen(vbox.topLeft()),
                                view->GraphToScreen(vbox.bottomRight()));
                            QString cur = (i < node.PinDefaultValues.size())
                                ? node.PinDefaultValues[i] : "0.0";
                            emit InlineEditRequested(m_EditTarget, cur);
                            view->update(); e->accept(); return;
                        }
                    }
                    pinY += k_PinRow;
                }
            }

            if (m_EditTarget.NodeId != -1)
            {
                emit InlineEditFinished(); m_EditTarget = {};
            }

            int pinIdx = -1;
            int pinNodeId = PinAt(gp, pinIdx);
            if (pinNodeId != -1)
            {
                const WeaveNode* n = FindNode(pinNodeId);
                if (n && n->Pins[pinIdx].IsOutput)
                {
                    m_DrawingWire = true;
                    m_WireFromNode = pinNodeId;
                    m_WireFromPin = pinIdx;
                    m_WireEndPos = gp;
                    return;
                }
            }

            int idx = NodeIndexAt(gp);
            if (idx != -1)
            {
                int clickedId = m_Nodes[idx].Id;
                if ((e->modifiers() & Qt::ShiftModifier) &&
                    m_SelectedNodeIds.count(clickedId))
                {
                    m_SelectedNodeIds.erase(clickedId);
                    m_DragNodeId = -1;
                }
                else
                {
                    if (!m_SelectedNodeIds.count(clickedId) &&
                        !(e->modifiers() & Qt::ShiftModifier))
                        m_SelectedNodeIds.clear();
                    m_SelectedNodeIds.insert(clickedId);
                    m_DragNodeId = clickedId;
                    m_LastDragMousePos = gp - m_Nodes[idx].Position;
                    WeaveNode node = m_Nodes.takeAt(idx);
                    m_Nodes.append(node);
                }
            }
            else
            {
                m_DragNodeId = -1;
                if (!(e->modifiers() & Qt::ShiftModifier))
                    m_SelectedNodeIds.clear();
                m_IsBoxSelecting = true;
                m_BoxSelectStart = gp;
                m_SelectionBox = QRectF(gp, QSizeF(0, 0));
            }
        }
        view->update();
    }

    void WeaveGraphScene::HandleMouseMove(QMouseEvent* e, WeaveGraphView* view)
    {
        QPointF gp = view->ScreenToGraph(e->position());

        if (m_DrawingWire) { m_WireEndPos = gp; view->update(); return; }
        if (m_IsBoxSelecting)
        {
            m_SelectionBox = QRectF(m_BoxSelectStart, gp).normalized(); view->update(); return;
        }

        if (m_DragNodeId != -1 && (e->buttons() & Qt::LeftButton))
        {
            if (HasActiveEdit()) { CancelEdit(); emit InlineEditFinished(); }
            WeaveNode* dragNode = FindNode(m_DragNodeId);
            if (dragNode)
            {
                QPointF newPos = gp - m_LastDragMousePos;
                QPointF delta = newPos - dragNode->Position;
                for (int id : m_SelectedNodeIds)
                    if (WeaveNode* n = FindNode(id)) n->Position += delta;
                view->update();
            }
        }
    }

    void WeaveGraphScene::HandleMouseRelease(QMouseEvent* e, WeaveGraphView* view)
    {
        QPointF gp = view->ScreenToGraph(e->position());

        if (m_DrawingWire && e->button() == Qt::LeftButton)
        {
            int pinIdx = -1;
            int pinNodeId = PinAt(gp, pinIdx);
            if (pinNodeId != -1 && pinNodeId != m_WireFromNode)
            {
                const WeaveNode* tn = FindNode(pinNodeId);
                if (tn && !tn->Pins[pinIdx].IsOutput)
                {
                    WeaveConnection conn;
                    conn.FromNodeId = m_WireFromNode;
                    conn.FromPinIdx = m_WireFromPin;
                    conn.ToNodeId = pinNodeId;
                    conn.ToPinIdx = pinIdx;
                    emit ConnectionRequested(conn);
                }
            }
            m_DrawingWire = false;
            view->update();
            return;
        }

        if (m_IsBoxSelecting && e->button() == Qt::LeftButton)
        {
            m_IsBoxSelecting = false;
            m_DragNodeId = -1;
            for (const auto& node : m_Nodes)
                if (m_SelectionBox.intersects(QRectF(node.Position, node.Size)))
                    m_SelectedNodeIds.insert(node.Id);
            view->update();
        }
    }

    void WeaveGraphScene::HandleKeyPress(QKeyEvent* e, WeaveGraphView* view)
    {
        if (e->key() == Qt::Key_Delete && !m_SelectedNodeIds.empty())
        {
            std::vector<int> toDelete(m_SelectedNodeIds.begin(),
                m_SelectedNodeIds.end());
            for (int id : toDelete) emit NodeRemoveRequested(id);
            m_SelectedNodeIds.clear();
            m_DragNodeId = -1;
            view->update();
            e->accept();
        }
        else if (e->key() == Qt::Key_Escape && m_EditTarget.NodeId != -1)
        {
            CancelEdit();
            emit InlineEditFinished();
            view->update();
        }
    }

    void WeaveGraphScene::CommitEdit(const QString& value)
    {
        if (m_EditTarget.NodeId == -1) return;
        WeaveNode* node = FindNode(m_EditTarget.NodeId);
        if (node && m_EditTarget.PinIdx >= 0 &&
            m_EditTarget.PinIdx < node->PinDefaultValues.size())
        {
            node->PinDefaultValues[m_EditTarget.PinIdx] = value;
            if (node->Kind == WeaveNodeKind::Const_Float && m_EditTarget.PinIdx == 0)
                node->Subtitle = value;
            emit SceneChanged();
        }
        m_EditTarget = {};
        emit InlineEditFinished();
    }

    void WeaveGraphScene::CancelEdit()
    {
        m_EditTarget = {};
        emit InlineEditFinished();
    }

} // namespace NuEditor::Weave