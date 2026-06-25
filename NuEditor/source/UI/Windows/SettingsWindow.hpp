#pragma once

#include <App/ResizableWindow.hpp>
#include <UI/Panels/CustomTitleBar.hpp>
#include <Core/EditorPreferences.hpp>

#include <QStackedWidget>
#include <QListWidget>
#include <QButtonGroup>

namespace NuEditor
{
	class SettingsWindow : public ResizableWindow
	{
		Q_OBJECT
	public:
		explicit SettingsWindow(QWidget* parent = nullptr);

	signals:
		void ThemeChanged();

	private slots:
		void OnApply();

	private:
		void SetupTitleBar();
		void SetupUI();
		void BuildAppearancePage();

		CustomTitleBar* m_TitleBar = nullptr;
		QListWidget* m_NavList = nullptr;
		QStackedWidget* m_Pages = nullptr;
		QButtonGroup* m_ThemeGroup = nullptr;

		EditorTheme m_SelectedTheme;
	};
} // namespace NuEditor