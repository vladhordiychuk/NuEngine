#include <UI/Panels/ContentDrawerPanel.hpp>

namespace NuEditor
{
	ContentDrawerPanel::ContentDrawerPanel(QWidget* parent)
		: QWidget(parent)
	{
		SetupUI();
	}

	void ContentDrawerPanel::SetupUI()
	{
		auto* layout = new QVBoxLayout(this);
		layout->setContentsMargins(8, 8, 8, 8);
		auto* label = new QLabel("Content Drawer");
		label->setAlignment(Qt::AlignCenter);
		layout->addWidget(label);
	}
} // namespace NuEditor