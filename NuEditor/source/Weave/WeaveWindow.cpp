#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QColor>
#include <QShortcut>
#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include <QTimer>

#include <Weave/WeaveWindow.hpp>
#include <Weave/WeaveCompiler.hpp>
#include <Weave/WeaveSerializer.hpp>

namespace NuEditor::Weave
{
    WeaveWindow::WeaveWindow(QWidget* parent)
        : ResizableWindow(parent)
    {
        setMinimumSize(800, 500);
        setWindowTitle("Weave - NuEditor");

        SetupUI();
        SpawnDemoNodes();
    }

    void WeaveWindow::SetupUI()
    {
        m_Scene = new WeaveGraphScene(this);
        m_CmdStack = new WeaveCommandStack(m_Scene, this);

        SetupTitleBar();

        m_ToolBar = new WeaveToolBar(this);
        addToolBar(Qt::TopToolBarArea, m_ToolBar);
        installMouseTracking(m_ToolBar);

        connect(m_ToolBar, &WeaveToolBar::CompileRequested, this, &WeaveWindow::OnCompile);
        connect(m_ToolBar, &WeaveToolBar::SaveRequested, this, &WeaveWindow::OnSave);
        connect(m_ToolBar, &WeaveToolBar::AddNodeRequested, this, &WeaveWindow::OnAddNode);
        connect(m_ToolBar, &WeaveToolBar::ZoomToFitRequested, this, &WeaveWindow::OnZoomToFit);
        connect(m_ToolBar, &WeaveToolBar::ToggleMinimapRequested, this, &WeaveWindow::OnToggleMinimap);

        m_GraphView = new WeaveGraphView(this);
        m_GraphView->SetScene(m_Scene);

        setCentralWidget(m_GraphView);
        installMouseTracking(m_GraphView);

        connect(m_Scene, &WeaveGraphScene::SceneChanged, this, &WeaveWindow::OnGraphChanged);

        connect(m_Scene, &WeaveGraphScene::ConnectionRequested, this, &WeaveWindow::OnConnectionRequested);
        connect(m_Scene, &WeaveGraphScene::NodeRemoveRequested, this, &WeaveWindow::OnNodeRemoveRequested);
        connect(m_Scene, &WeaveGraphScene::DisconnectRequested, this, &WeaveWindow::OnDisconnectRequested);

        connect(m_Scene, &WeaveGraphScene::SpawnNodeRequested,
            this, [this](WeaveNodeKind kind, QPointF pos) {
                const auto* def = WeaveNodeRegistry::Get().Find(kind);
                if (!def) return;
                WeaveNode node;
                node.Kind = kind;
                node.Title = def->Title;
                node.HeaderColor = def->HeaderColor;
                node.Pins = def->Pins;
                node.PinDefaultValues = def->DefaultValues;
                node.Position = pos;
                while (node.PinDefaultValues.size() < node.Pins.size())
                    node.PinDefaultValues.append("");

                m_CmdStack->Push(std::make_unique<CmdAddNode>(node));
                m_GraphView->update();
            });
    }

    void WeaveWindow::SetupTitleBar()
    {
        m_TitleBar = new CustomTitleBar(this);
        QMenuBar* mb = m_TitleBar->MenuBar();

        QMenu* fileMenu = mb->addMenu("File");

        m_OpenAction = fileMenu->addAction(QIcon(":/icons/open_scene.svg"), "Open Graph...");
        m_SaveAction = fileMenu->addAction(QIcon(":/icons/save_scene.svg"), "Save");
        m_SaveAsAction = fileMenu->addAction(QIcon(":/icons/save_as_scene.svg"), "Save As...");

        m_OpenAction->setShortcut(QKeySequence::Open);
        m_SaveAction->setShortcut(QKeySequence::Save);
        m_SaveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));

        this->addAction(m_OpenAction);
        this->addAction(m_SaveAction);
        this->addAction(m_SaveAsAction);

        fileMenu->addSeparator();
        QAction* exitAction = fileMenu->addAction("Close Weave");

        connect(m_OpenAction, &QAction::triggered, this, &WeaveWindow::OnOpen);
        connect(m_SaveAction, &QAction::triggered, this, &WeaveWindow::OnSave);
        connect(m_SaveAsAction, &QAction::triggered, this, &WeaveWindow::OnSaveAs);
        connect(exitAction, &QAction::triggered, this, &WeaveWindow::close);

        QMenu* editMenu = mb->addMenu("Edit");

        m_UndoAction = editMenu->addAction("Undo");
        m_UndoAction->setShortcut(QKeySequence::Undo);
        m_UndoAction->setEnabled(false);

        m_RedoAction = editMenu->addAction("Redo");
        m_RedoAction->setShortcut(QKeySequence::Redo);
        m_RedoAction->setEnabled(false);

        editMenu->addSeparator();
        QAction* cutAction = editMenu->addAction("Cut");
        QAction* copyAction = editMenu->addAction("Copy");
        QAction* pasteAction = editMenu->addAction("Paste");
        QAction* selAllAction = editMenu->addAction("Select All");

        cutAction->setShortcut(QKeySequence::Cut);
        copyAction->setShortcut(QKeySequence::Copy);
        pasteAction->setShortcut(QKeySequence::Paste);
        selAllAction->setShortcut(QKeySequence::SelectAll);

        this->addAction(m_UndoAction);
        this->addAction(m_RedoAction);
        this->addAction(cutAction);
        this->addAction(copyAction);
        this->addAction(pasteAction);
        this->addAction(selAllAction);

        connect(m_UndoAction, &QAction::triggered, this, &WeaveWindow::OnUndo);
        connect(m_RedoAction, &QAction::triggered, this, &WeaveWindow::OnRedo);
        connect(cutAction, &QAction::triggered, this, &WeaveWindow::OnCut);
        connect(copyAction, &QAction::triggered, this, &WeaveWindow::OnCopy);
        connect(pasteAction, &QAction::triggered, this, &WeaveWindow::OnPaste);
        connect(selAllAction, &QAction::triggered, this, &WeaveWindow::OnSelectAll);

        connect(m_CmdStack, &WeaveCommandStack::HistoryChanged, this, [this]() {
            m_UndoAction->setEnabled(m_CmdStack->CanUndo());
            m_RedoAction->setEnabled(m_CmdStack->CanRedo());
            m_UndoAction->setText("Undo " + m_CmdStack->UndoDescription());
            m_RedoAction->setText("Redo " + m_CmdStack->RedoDescription());
            });

        connect(m_TitleBar, &CustomTitleBar::MinimizeRequested, this, &QWidget::showMinimized);
        connect(m_TitleBar, &CustomTitleBar::MaximizeRequested, this, &ResizableWindow::ToggleCustomMaximize);
        connect(m_TitleBar, &CustomTitleBar::CloseRequested, this, &QWidget::close);

        connect(this, &ResizableWindow::customMaximizeStateChanged, this, [this](bool isMaximized) {
            if (m_TitleBar) m_TitleBar->ForceMaxIconState(isMaximized);
            });

        setMenuWidget(m_TitleBar);
        installMouseTracking(m_TitleBar);
    }

    void WeaveWindow::OnCompile()
    {
        WeaveCompiler compiler;
        CompileResult result = compiler.Compile(*m_Scene, "test_script.wbc");

        if (result.HasErrors())
        {
            QString errStr = "Compilation failed:\n\n";
            for (const auto& diag : result.Diagnostics)
            {
                if (diag.Severity == DiagSeverity::Error)
                {
                    errStr += QString::fromStdString(diag.Message) + "\n";
                }
            }
            QMessageBox::critical(this, "Compile Error", errStr);
        }
        else
        {
            QMessageBox::information(this, "Compile Success",
                QString("Script successfully compiled!\nSaved %1 bytes to test_script.wbc")
                .arg(result.Bytecode.size()));
        }
    }

    void WeaveWindow::OnGraphChanged()
    {
        if (!m_IsDirty)
        {
            m_IsDirty = true;
            setWindowTitle(windowTitle() + "*");
        }
    }

    void WeaveWindow::OnOpen()
    {
        if (m_IsDirty)
        {
            QMessageBox::StandardButton res = QMessageBox::warning(this, "Unsaved Changes",
                "You have unsaved changes in your current graph.\nDo you want to save them before opening a new file?",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

            if (res == QMessageBox::Save)
            {
                OnSave();
                if (m_IsDirty) return;
            }
            else if (res == QMessageBox::Cancel)
            {
                return;
            }
        }

        QString path = QFileDialog::getOpenFileName(
            this, "Open Weave Graph", QString(), "Weave Graph (*.wgraph)");

        if (!path.isEmpty())
        {
            LoadGraph(path);
        }
    }

    void WeaveWindow::OnSave()
    {
        if (m_CurrentFilePath.isEmpty())
        {
            m_CurrentFilePath = QFileDialog::getSaveFileName(
                this, "Save Weave Graph", QString(), "Weave Graph (*.wgraph)");
            if (m_CurrentFilePath.isEmpty())
            {
                return;
            }
        }

        auto result = WeaveSerializer::Save(*m_Scene, m_CurrentFilePath);

        if (!result.Success)
        {
            QMessageBox::critical(this, "Save Error", result.Error);
            return;
        }
        m_IsDirty = false;
        QFileInfo fi(m_CurrentFilePath);
        setWindowTitle("Weave - " + fi.baseName());
    }

    void WeaveWindow::OnSaveAs()
    {
        QString path = QFileDialog::getSaveFileName(
            this, "Save Weave Graph As", QString(), "Weave Graph (*.wgraph)");

        if (path.isEmpty())
        {
            return;
        }

        m_CurrentFilePath = path;
        OnSave();
    }

    void WeaveWindow::OnAddNode()
    {
        WeaveNode n;
        n.Title = "New Node";
        n.HeaderColor = QColor(0x44, 0x44, 0x44);
        n.Position = m_GraphView->ScreenToGraph(
            QPointF(m_GraphView->width() * 0.5f, m_GraphView->height() * 0.5f));
        n.Pins.append({ "In",  false, QColor(0xFF, 0xFF, 0xFF) });
        n.Pins.append({ "Out", true,  QColor(0xFF, 0xFF, 0xFF) });

        m_CmdStack->Push(std::make_unique<CmdAddNode>(n));
        m_GraphView->update();
    }

    void WeaveWindow::OnZoomToFit()
    {
        m_GraphView->ZoomToFit();
    }

    void WeaveWindow::OnToggleMinimap(bool visible)
    {
        m_GraphView->SetMinimapVisible(visible);
        m_GraphView->update();
    }

    void WeaveWindow::LoadGraph(const QString& path)
    {
        auto result = WeaveSerializer::Load(*m_Scene, path);

        if (!result.Success)
        {
            QMessageBox::critical(this, "Load Error", result.Error);
            return;
        }

        m_CurrentFilePath = path;
        m_IsDirty = false;

        m_CmdStack->Clear();

        QFileInfo fi(path);
        setWindowTitle("Weave - " + fi.baseName());

        if (!result.Error.isEmpty())
            QMessageBox::warning(this, "Load Warnings", result.Error);

        m_GraphView->ZoomToFit();
        m_GraphView->update();
    }

    void WeaveWindow::closeEvent(QCloseEvent* event)
    {
        if (m_IsDirty)
        {
            QMessageBox::StandardButton res = QMessageBox::warning(this, "Unsaved Changes",
                "You have unsaved changes in your graph.\nDo you want to save them before closing?",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

            if (res == QMessageBox::Save)
            {
                OnSave();
                if (m_IsDirty)
                {
                    event->ignore();
                    return;
                }
            }
            else if (res == QMessageBox::Cancel)
            {
                event->ignore();
                return;
            }
        }
        event->accept();
    }

    void WeaveWindow::OnUndo()
    {
        if (m_CmdStack->CanUndo())
        {
            m_CmdStack->Undo();
            m_GraphView->update();
        }
    }

    void WeaveWindow::OnRedo()
    {
        if (m_CmdStack->CanRedo())
        {
            m_CmdStack->Redo();
            m_GraphView->update();
        }
    }

    void WeaveWindow::OnCopy()
    {
        const auto& sel = m_Scene->GetSelection();
        if (sel.isEmpty()) 
        {
            return;
        }
        WeaveClipboard::Get().Copy(*m_Scene, sel);

        m_PasteCounter = 1;
    }

    void WeaveWindow::OnPaste()
    {
        if (!WeaveClipboard::Get().HasData()) 
        {
            return;
        }

        QPointF offset(30.0f * m_PasteCounter, 30.0f * m_PasteCounter);
        m_PasteCounter++;

        auto cmd = WeaveClipboard::Get().MakePasteCommand(offset);
        auto* rawCmd = cmd.get();
        m_CmdStack->Push(std::move(cmd));

        m_Scene->SetSelection(rawCmd->NewIds());
        m_GraphView->update();
    }

    void WeaveWindow::OnCut()
    {
        OnCopy();

        for (int id : m_Scene->GetSelection())
        {
            m_CmdStack->Push(std::make_unique<CmdRemoveNode>(id));
        }
        m_Scene->ClearSelection();
        m_GraphView->update();
    }

    void WeaveWindow::OnSelectAll()
    {
        QVector<int> all;
        for (const auto& n : m_Scene->GetNodes())
        {
            all.append(n.Id);
        }
        m_Scene->SetSelection(all);
        m_GraphView->update();
    }

    void WeaveWindow::OnConnectionRequested(WeaveConnection conn)
    {
        m_CmdStack->Push(std::make_unique<CmdAddConnection>(conn));
        m_GraphView->update();
    }

    void WeaveWindow::OnNodeRemoveRequested(int nodeId)
    {
        m_CmdStack->Push(std::make_unique<CmdRemoveNode>(nodeId));
        m_GraphView->update();
    }

    void WeaveWindow::OnDisconnectRequested(WeaveConnection conn)
    {
        m_CmdStack->Push(std::make_unique<CmdRemoveConnection>(conn));
        m_GraphView->update();
    }

    void WeaveWindow::OnResizeAnimationStart()
    {
        if (m_GraphView)
        {
            m_GraphView->SetFreezeFrame(true);
        }
    }

    void WeaveWindow::OnResizeAnimationEnd()
    {
        if (m_GraphView)
        {
            m_GraphView->SetFreezeFrame(false);
        }
    }

} // namespace NuEditor::Weave