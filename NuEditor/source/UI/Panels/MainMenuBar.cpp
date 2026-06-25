#include <QApplication>

#include <UI/Panels/MainMenuBar.hpp>

namespace NuEditor
{
	MainMenuBar::MainMenuBar(QMenuBar* target)
		: QObject(target), m_MenuBar(target)
	{
		SetupFileMenu();
		SetupEditMenu();
		SetupWindowMenu();
		SetupToolsMenu();
		SetupBuildMenu();
		SetupHelpMenu();
		SetupSettingsMenu();
	}

	void MainMenuBar::SetupFileMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("File");

		QAction* newSceneAction = menu->addAction("New Scene");
		QAction* openSceneAction = menu->addAction("Open Scene");
		menu->addSeparator();
		QAction* saveAction = menu->addAction("Save Scene");
		QAction* saveAsAction = menu->addAction("Save Scene As...");
		menu->addSeparator();
		QAction* exitAction = menu->addAction("Exit");

		connect(newSceneAction, &QAction::triggered, this, &MainMenuBar::NewSceneRequested);
		connect(openSceneAction, &QAction::triggered, this, &MainMenuBar::OpenSceneRequested);
		connect(saveAction, &QAction::triggered, this, &MainMenuBar::SaveSceneRequested);
		connect(saveAsAction, &QAction::triggered, this, &MainMenuBar::SaveAsRequested);
		connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
	}

	void MainMenuBar::SetupEditMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("Edit");

		QAction* undoAction = menu->addAction("Undo");
		undoAction->setShortcut(QKeySequence::Undo);

		QAction* redoAction = menu->addAction("Redo");
		redoAction->setShortcut(QKeySequence::Redo);

		QAction* cutAction = menu->addAction("Cut");
		cutAction->setShortcut(QKeySequence::Cut);

		QAction* copyAction = menu->addAction("Copy");
		copyAction->setShortcut(QKeySequence::Copy);
		QAction* pasteAction = menu->addAction("Paste");
		pasteAction->setShortcut(QKeySequence::Paste);
		menu->addSeparator();
		QAction* editorPreferencesAction = menu->addAction("Editor Preferences");
	}

	void MainMenuBar::SetupWindowMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("Window");

		QAction* toggleSceneAction = menu->addAction("Scene Hierarchy");
		QAction* toggleInspectorAction = menu->addAction("Inspector");

		connect(toggleSceneAction, &QAction::triggered, this, &MainMenuBar::ToggleSceneRequested);
		connect(toggleInspectorAction, &QAction::triggered, this, &MainMenuBar::ToggleInspectorRequested);
	}

	void MainMenuBar::SetupToolsMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("Tools");

		QAction* nodeEditorAction = menu->addAction("Weave");
		menu->addSeparator();
		QAction* assetImportAction = menu->addAction("Asset Import...");
	}

	void MainMenuBar::SetupBuildMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("Build");

		QAction* buildProjectAction = menu->addAction("Build Project");
		QAction* buildAndRunAction = menu->addAction("Build and Run");
		menu->addSeparator();
		QAction* packageProjectAction = menu->addAction("Package Project...");
	}

	void MainMenuBar::SetupHelpMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("Help");

		QAction* documentationAction = menu->addAction("Documentation");
		QAction* aboutEditorAction = menu->addAction("About Editor");
	}

	void MainMenuBar::SetupSettingsMenu()
	{
		QMenu* menu = m_MenuBar->addMenu("Settings");
	}
} // namespace NuEditor