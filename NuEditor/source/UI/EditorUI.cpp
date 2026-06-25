#include <QMainWindow>
#include <QStatusBar>
#include <QToolButton>
#include <QApplication>
#include <QFile>
#include <QTextStream>

#include <UI/EditorUI.hpp>
#include <UI/Windows/SettingsWindow.hpp>

#include <Core/EditorPreferences.hpp>

namespace NuEditor
{
    EditorUI::EditorUI(QMainWindow* host, QWidget* viewport, QObject* parent)
        : QObject(parent), m_Host(host)
    {
        BuildTitleBar();
        BuildToolBar();
        BuildCentralWidget(viewport);
        BuildStatusBar();
        BuildDockPanels();
    }

    QWidget* EditorUI::CentralWidget() const
    {
        return m_Splitter;
    }

    void EditorUI::BuildTitleBar()
    {
        m_TitleBar = new CustomTitleBar(m_Host);
        QMenuBar* mb = m_TitleBar->MenuBar();

        QMenu* fileMenu = mb->addMenu("File");
        m_NewSceneAction = fileMenu->addAction(QIcon(":/icons/new_scene.svg"), "New Scene");
        m_OpenSceneAction = fileMenu->addAction(QIcon(":/icons/open_scene.svg"), "Open Scene");
        m_LoadDllAction = fileMenu->addAction(QIcon(":/icons/dll.svg"), "Load Game DLL...");
        fileMenu->addSeparator();
        m_SaveAction = fileMenu->addAction(QIcon(":/icons/save.svg"), "Save Scene");
        fileMenu->addSeparator();
        m_ExitAction = fileMenu->addAction(QIcon(":/icons/exit.svg"), "Exit");

        m_NewSceneAction->setShortcut(QKeySequence::New);
        m_OpenSceneAction->setShortcut(QKeySequence::Open);
        m_SaveAction->setShortcut(QKeySequence::Save);

        connect(m_LoadDllAction, &QAction::triggered, this, &EditorUI::LoadDllRequested);
        connect(m_NewSceneAction, &QAction::triggered, this, &EditorUI::NewSceneRequested);
        connect(m_OpenSceneAction, &QAction::triggered, this, &EditorUI::OpenSceneRequested);
        connect(m_SaveAction, &QAction::triggered, this, &EditorUI::SaveSceneRequested);
        connect(m_ExitAction, &QAction::triggered, m_Host, &QWidget::close);

        QMenu* editMenu = mb->addMenu("Edit");
        QAction* undoAction = editMenu->addAction("Undo");
        QAction* redoAction = editMenu->addAction("Redo");
        undoAction->setShortcut(QKeySequence::Undo);
        redoAction->setShortcut(QKeySequence::Redo);
        editMenu->addSeparator();
        m_EditorPrefsAction = editMenu->addAction("Editor Preferences...");

        QMenu* windowMenu = mb->addMenu("Window");
        m_ToggleSceneAction = windowMenu->addAction("Scene Hierarchy");
        m_ToggleInspectorAction = windowMenu->addAction("Inspector");
        m_ToggleSceneAction->setCheckable(true);
        m_ToggleInspectorAction->setCheckable(true);

        connect(m_ToggleSceneAction, &QAction::triggered, this, [this](bool checked) {
            m_ScenePanel->setVisible(checked);
            });
        connect(m_ToggleInspectorAction, &QAction::triggered, this, [this](bool checked) {
            m_InspectorPanel->setVisible(checked);
            });

        QMenu* toolsMenu = mb->addMenu("Tools");
        m_NodeEditorAction = toolsMenu->addAction("Node Editor");
        connect(m_NodeEditorAction, &QAction::triggered,
            this, &EditorUI::NodeEditorRequested);

        QMenu* buildMenu = mb->addMenu("Build");
        QAction* buildAction = buildMenu->addAction("Build Project");
        buildAction->setShortcut(QKeySequence("Ctrl+B"));

        QMenu* helpMenu = mb->addMenu("Help");
        helpMenu->addAction("Documentation");
        helpMenu->addAction("About NuEditor");

        connect(m_TitleBar, &CustomTitleBar::MinimizeRequested,
            m_Host, &QWidget::showMinimized);
        connect(m_TitleBar, &CustomTitleBar::MaximizeRequested,
            m_Host, [this]() {
                auto* rw = qobject_cast<ResizableWindow*>(m_Host);
                if (rw) rw->ToggleCustomMaximize();
            });
        connect(m_TitleBar, &CustomTitleBar::CloseRequested,
            m_Host, &QWidget::close);

        connect(m_EditorPrefsAction, &QAction::triggered, this, [this]() {
            if (!m_SettingsWindow)
            {
                m_SettingsWindow = new SettingsWindow(m_Host);
                m_SettingsWindow->setAttribute(Qt::WA_DeleteOnClose);
                m_SettingsWindow->resize(620, 440);
                connect(m_SettingsWindow, &SettingsWindow::ThemeChanged,
                    this, &EditorUI::ThemeChanged);
            }
            m_SettingsWindow->show();
            m_SettingsWindow->raise();
            m_SettingsWindow->activateWindow();
            });

        m_Host->setMenuWidget(m_TitleBar);
    }

    void EditorUI::BuildToolBar()
    {
        m_ToolBar = new MainToolBar(m_Host);
        auto* mw = qobject_cast<QMainWindow*>(m_Host);
        if (mw) mw->addToolBar(Qt::TopToolBarArea, m_ToolBar);

        connect(m_ToolBar, &MainToolBar::SettingsRequested,
            m_EditorPrefsAction, &QAction::trigger);
        connect(m_ToolBar, &MainToolBar::NewSceneRequested,
            m_NewSceneAction, &QAction::trigger);
        connect(m_ToolBar, &MainToolBar::OpenSceneRequested,
            m_OpenSceneAction, &QAction::trigger);
        connect(m_ToolBar, &MainToolBar::SaveSceneRequested,
            m_SaveAction, &QAction::trigger);

        connect(m_ToolBar, &MainToolBar::PlayRequested,
            this, &EditorUI::PlayRequested);
        connect(m_ToolBar, &MainToolBar::PauseRequested,
            this, &EditorUI::PauseRequested);
        connect(m_ToolBar, &MainToolBar::StopRequested,
            this, &EditorUI::StopRequested);
    }

    void EditorUI::BuildCentralWidget(QWidget* viewport)
    {
        m_Splitter = new QSplitter(Qt::Vertical, m_Host);
        m_Splitter->setHandleWidth(4);
        m_Splitter->setChildrenCollapsible(true);

        m_Splitter->addWidget(viewport);

        m_BottomTabs = new QTabWidget(m_Splitter);
        m_BottomTabs->setTabPosition(QTabWidget::South);
        m_BottomTabs->setDocumentMode(true);
        m_BottomTabs->setObjectName("bottomTabs");
        m_BottomTabs->setMaximumHeight(300);
        m_BottomTabs->tabBar()->hide();
        m_BottomTabs->setVisible(false);

        m_ContentDrawer = new ContentDrawerPanel(m_BottomTabs);
        m_ConsolePanel = new ConsolePanel(m_BottomTabs);

        m_BottomTabs->addTab(m_ContentDrawer,
            QIcon(":/icons/content.svg"), "Content Drawer");
        m_BottomTabs->addTab(m_ConsolePanel,
            QIcon(":/icons/console.svg"), "Output Log");

        m_ConsolePanel->ConnectToLogger();

        m_Splitter->addWidget(m_BottomTabs);
        m_Splitter->setStretchFactor(0, 3);
        m_Splitter->setStretchFactor(1, 1);

        auto* mw = qobject_cast<QMainWindow*>(m_Host);
        if (mw) mw->setCentralWidget(m_Splitter);
    }

    void EditorUI::BuildStatusBar()
    {
        auto* mw = qobject_cast<QMainWindow*>(m_Host);
        if (!mw) return;

        auto* contentBtn = new QToolButton(m_Host);
        contentBtn->setText("Content Drawer");
        contentBtn->setCheckable(true);
        contentBtn->setObjectName("statusToggleBtn");

        auto* outputBtn = new QToolButton(m_Host);
        outputBtn->setText("Output Log");
        outputBtn->setCheckable(true);
        outputBtn->setObjectName("statusToggleBtn");

        mw->statusBar()->addWidget(contentBtn);
        mw->statusBar()->addWidget(outputBtn);

        connect(contentBtn, &QToolButton::toggled,
            this, [this, outputBtn](bool checked) {
                if (checked) {
                    outputBtn->setChecked(false);
                    m_BottomTabs->setCurrentIndex(0);
                    m_BottomTabs->setVisible(true);
                }
                else {
                    m_BottomTabs->setVisible(false);
                }
            });
        connect(outputBtn, &QToolButton::toggled,
            this, [this, contentBtn](bool checked) {
                if (checked) {
                    contentBtn->setChecked(false);
                    m_BottomTabs->setCurrentIndex(1);
                    m_BottomTabs->setVisible(true);
                }
                else {
                    m_BottomTabs->setVisible(false);
                }
            });

        auto* savedLabel = new QLabel("✔ All Saved", m_Host);
        savedLabel->setObjectName("statusLabel");
        mw->statusBar()->addWidget(savedLabel);

        m_FpsLabel = new QLabel("FPS: 0 | Frame Time: 0.00 ms", m_Host);
        m_FpsLabel->setObjectName("fpsLabel");
        mw->statusBar()->addPermanentWidget(m_FpsLabel);
    }

    void EditorUI::BuildDockPanels()
    {
        auto* mw = qobject_cast<QMainWindow*>(m_Host);
        if (!mw) return;

        m_ScenePanel = new SceneHierarchyPanel(m_Host);
        m_ScenePanel->setFeatures(
            QDockWidget::DockWidgetMovable |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetClosable);
        mw->addDockWidget(Qt::LeftDockWidgetArea, m_ScenePanel);
        m_ScenePanel->hide();

        m_InspectorPanel = new InspectorPanel(m_Host);
        m_InspectorPanel->setFeatures(
            QDockWidget::DockWidgetMovable |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetClosable);
        mw->addDockWidget(Qt::RightDockWidgetArea, m_InspectorPanel);
        m_InspectorPanel->hide();

        mw->resizeDocks(
            { m_ScenePanel, m_InspectorPanel },
            { 250, 250 },
            Qt::Horizontal);

        connect(m_ScenePanel, &QDockWidget::visibilityChanged,
            this, [this](bool visible) {
                if (m_ToggleSceneAction)
                    m_ToggleSceneAction->setChecked(visible);
            });
        connect(m_InspectorPanel, &QDockWidget::visibilityChanged,
            this, [this](bool visible) {
                if (m_ToggleInspectorAction)
                    m_ToggleInspectorAction->setChecked(visible);
            });
    }

    void EditorUI::SetEditorState(EditorState state)
    {
        m_ToolBar->SetEditorState(state);
    }

    void EditorUI::SetFps(float fps, float ms)
    {
        if (!m_FpsLabel) return;

        QString color = (fps >= 60) ? "#00FF00"
            : (fps >= 30) ? "#FFAA00"
            : "#FF0000";
        m_FpsLabel->setStyleSheet(
            QString("padding: 2px 8px; color: %1; font-weight: bold;")
            .arg(color));
        m_FpsLabel->setText(
            QString("FPS: %1 | Frame Time: %2 ms")
            .arg(static_cast<int>(fps))
            .arg(ms, 0, 'f', 2));
    }
}