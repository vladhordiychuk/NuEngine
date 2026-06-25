#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

namespace NuEditor
{
	class ContentDrawerPanel : public QWidget
	{
		Q_OBJECT
	public:
		explicit ContentDrawerPanel(QWidget* parent = nullptr);

	private:
		void SetupUI();
	};
} // namespace NuEditor