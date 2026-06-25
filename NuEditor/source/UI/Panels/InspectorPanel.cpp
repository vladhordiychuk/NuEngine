#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

#include <UI/Panels/InspectorPanel.hpp>

namespace NuEditor
{
    InspectorPanel::InspectorPanel(QWidget* parent)
        : QDockWidget("PROPERTIES", parent)
    {
        setFeatures(QDockWidget::DockWidgetMovable |
            QDockWidget::DockWidgetFloatable       |
            QDockWidget::DockWidgetClosable);
        SetupUI();
    }

    void InspectorPanel::SetupUI()
    {
        m_StackWidget = new QStackedWidget(this);
        setWidget(m_StackWidget);

        m_EmptyPage = new QWidget();
        QVBoxLayout* emptyLayout = new QVBoxLayout(m_EmptyPage);
        emptyLayout->setContentsMargins(0, 0, 0, 0);

        m_EmptyLabel = new QLabel("No Object Selected");
        m_EmptyLabel->setAlignment(Qt::AlignCenter);
        m_EmptyLabel->setStyleSheet("color: gray; font-size: 14px;");
        m_EmptyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        emptyLayout->addWidget(m_EmptyLabel);

        m_DataPage = new QWidget();
        m_DataLayout = new QVBoxLayout(m_DataPage);
        m_DataLayout->setAlignment(Qt::AlignTop);

        m_ObjectNameLabel = new QLabel("Object Name");
        m_ObjectNameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        m_DataLayout->addWidget(m_ObjectNameLabel);

        m_StackWidget->addWidget(m_EmptyPage);
        m_StackWidget->addWidget(m_DataPage);
        m_StackWidget->setCurrentIndex(0);
    }

    void InspectorPanel::SetObjectName(const QString& name)
    {
        if (name.isEmpty())
        {
            m_StackWidget->setCurrentIndex(0);
        }
        else 
        {
            m_ObjectNameLabel->setText(name);
            m_StackWidget->setCurrentIndex(1);
        }
    }
} // namespace NuEditor