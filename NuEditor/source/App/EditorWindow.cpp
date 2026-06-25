#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>

#include <App/EditorWindow.hpp>
#include <Core/EditorPreferences.hpp>

namespace NuEditor
{
    EditorWindow::EditorWindow(QWidget* parent)
        : ResizableWindow(parent)
    {
        setMinimumSize(640, 480);

        m_Runtime = new EditorRuntime(this);

        m_Viewport = new EditorViewportHost(m_Runtime, this);

        m_UI = new EditorUI(this, m_Viewport, this);

        connect(m_UI, &EditorUI::PlayRequested,
            m_Runtime, &EditorRuntime::Play);
        connect(m_UI, &EditorUI::PauseRequested,
            m_Runtime, &EditorRuntime::Pause);
        connect(m_UI, &EditorUI::StopRequested,
            m_Runtime, &EditorRuntime::Stop);
        connect(m_UI, &EditorUI::LoadDllRequested,
            this, &EditorWindow::OnLoadDll);
        connect(m_UI, &EditorUI::NodeEditorRequested,
            this, &EditorWindow::OnNodeEditor);

        connect(m_Runtime, &EditorRuntime::StateChanged,
            m_UI, &EditorUI::SetEditorState);
        connect(m_Runtime, &EditorRuntime::FpsUpdated,
            m_UI, &EditorUI::SetFps);

        connect(m_Viewport->GLWidget(), &NuOpenGLWidget::glInitialized,
            this, [this]() {
                m_Runtime->StartRenderLoop();
            });

        connect(this, &ResizableWindow::customMaximizeStateChanged,
            this, [this](bool isMaximized) {
                m_UI->TitleBar()->ForceMaxIconState(isMaximized);
            });

        installMouseTracking(m_UI->TitleBar());
        installMouseTracking(m_UI->ToolBar());
        installMouseTracking(m_UI->Splitter());
    }

    EditorWindow::~EditorWindow()
    {
        m_UI->Console()->DisconnectFromLogger();

        if (m_WeaveWindow)
        {
            delete m_WeaveWindow;
            m_WeaveWindow = nullptr;
        }
    }

    void EditorWindow::OnLoadDll()
    {
        QString path = QFileDialog::getOpenFileName(
            this, "Load Game DLL", QString(), "Game Library (*.dll)");
        if (path.isEmpty()) return;

        if (!m_Runtime->LoadDll(path))
        {
            QMessageBox::critical(this, "Error",
                "Failed to load Game DLL: " + path);
        }
    }

    void EditorWindow::OnNodeEditor()
    {
        if (!m_WeaveWindow)
        {
            m_WeaveWindow = new Weave::WeaveWindow(nullptr);
            m_WeaveWindow->setAttribute(Qt::WA_DeleteOnClose, false);
            m_WeaveWindow->resize(1400, 900);
        }
        m_WeaveWindow->show();
        m_WeaveWindow->raise();
        m_WeaveWindow->activateWindow();
    }

    void EditorWindow::closeEvent(QCloseEvent* event)
    {

        ResizableWindow::closeEvent(event);
    }
}