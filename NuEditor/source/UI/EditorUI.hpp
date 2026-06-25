#pragma once

#include <QObject>
#include <QSplitter>
#include <QTabWidget>
#include <QLabel>
#include <QPointer>

#include <UI/Panels/CustomTitleBar.hpp>
#include <UI/Panels/MainToolBar.hpp>
#include <UI/Panels/SceneHierarchyPanel.hpp>
#include <UI/Panels/InspectorPanel.hpp>
#include <UI/Panels/ConsolePanel.hpp>
#include <UI/Panels/ContentDrawerPanel.hpp>
#include <UI/Windows/SettingsWindow.hpp>

#include <Core/EditorState.hpp>

class QMainWindow;

namespace NuEditor
{
    class EditorUI : public QObject
    {
        Q_OBJECT
    public:
        explicit EditorUI(QMainWindow* host, QWidget* viewport, QObject* parent = nullptr);

        [[nodiscard]] CustomTitleBar* TitleBar() const { return m_TitleBar; }
        [[nodiscard]] MainToolBar* ToolBar() const { return m_ToolBar; }
        [[nodiscard]] QSplitter* Splitter() const { return m_Splitter; }
        [[nodiscard]] ConsolePanel* Console() const { return m_ConsolePanel; }
        [[nodiscard]] SceneHierarchyPanel* ScenePanel() const { return m_ScenePanel; }
        [[nodiscard]] InspectorPanel* Inspector() const { return m_InspectorPanel; }
        [[nodiscard]] QLabel* FpsLabel() const { return m_FpsLabel; }

        [[nodiscard]] QWidget* CentralWidget() const;

        void SetEditorState(EditorState state);
        void SetFps(float fps, float ms);

    signals:
        void LoadDllRequested();
        void NewSceneRequested();
        void OpenSceneRequested();
        void SaveSceneRequested();
        void PlayRequested();
        void PauseRequested();
        void StopRequested();
        void NodeEditorRequested();
        void SettingsRequested();
        void ThemeChanged();

    private:
        void BuildTitleBar();
        void BuildToolBar();
        void BuildCentralWidget(QWidget* viewportPlaceholder);
        void BuildStatusBar();
        void BuildDockPanels();

        QMainWindow* m_Host = nullptr;
        CustomTitleBar* m_TitleBar = nullptr;
        MainToolBar* m_ToolBar = nullptr;
        QSplitter* m_Splitter = nullptr;
        QTabWidget* m_BottomTabs = nullptr;
        ConsolePanel* m_ConsolePanel = nullptr;
        ContentDrawerPanel* m_ContentDrawer = nullptr;
        SceneHierarchyPanel* m_ScenePanel = nullptr;
        InspectorPanel* m_InspectorPanel = nullptr;
        QPointer<SettingsWindow> m_SettingsWindow;
        QLabel* m_FpsLabel = nullptr;

        QAction* m_NewSceneAction = nullptr;
        QAction* m_OpenSceneAction = nullptr;
        QAction* m_SaveAction = nullptr;
        QAction* m_LoadDllAction = nullptr;
        QAction* m_NodeEditorAction = nullptr;
        QAction* m_EditorPrefsAction = nullptr;
        QAction* m_ToggleSceneAction = nullptr;
        QAction* m_ToggleInspectorAction = nullptr;
        QAction* m_ExitAction = nullptr;
    };
}