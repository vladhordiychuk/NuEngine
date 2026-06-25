#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

namespace NuEditor
{
	class InspectorPanel : public QDockWidget
	{
		Q_OBJECT
	public:
		explicit InspectorPanel(QWidget* parent = nullptr);

		void SetObjectName(const QString& name);
	private:
		QStackedWidget* m_StackWidget;

		QWidget* m_EmptyPage;
		QLabel* m_EmptyLabel;

		QWidget* m_DataPage;
		QVBoxLayout* m_DataLayout;
		QLabel* m_ObjectNameLabel;

		void SetupUI();

		[[nodiscard]] QVBoxLayout* CreateSection(const QString& title);
	};
} // namespace NuEditor