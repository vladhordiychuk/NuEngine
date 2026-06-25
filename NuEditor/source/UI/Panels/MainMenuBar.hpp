#pragma once

#include <QMenuBar>
#include <QMenu>
#include <QAction>

namespace NuEditor
{
	class MainMenuBar : public QObject
	{
		Q_OBJECT
	public:
		explicit MainMenuBar(QMenuBar* parent = nullptr);

	signals:
		void NewSceneRequested();
		void OpenSceneRequested();
		void SaveSceneRequested();
		void SaveAsRequested();
		void ToggleSceneRequested();
		void ToggleInspectorRequested();

	private:
		QMenuBar* m_MenuBar = nullptr;

		void SetupFileMenu();
		void SetupEditMenu();
		void SetupWindowMenu();
		void SetupToolsMenu();
		void SetupBuildMenu();
		void SetupHelpMenu();
		void SetupSettingsMenu();
	};
} // namespace NuEditor