#include <QHBoxLayout>
#include <QWidget>
#include <QIcon>
#include <QFrame>

#include <Weave/WeaveToolBar.hpp>

namespace NuEditor::Weave
{
    WeaveToolBar::WeaveToolBar(QWidget* parent)
        : QToolBar("Weave Toolbar", parent)
    {
        setMovable(false);
        setObjectName("weaveToolBar");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        SetupUI();
    }

    void WeaveToolBar::SetupUI()
    {
        m_GraphLabel = new QLabel("Untitled Graph", this);
        m_GraphLabel->setObjectName("weaveGraphLabel");
        addWidget(m_GraphLabel);

        addSeparator();

        m_CompileBtn = new QToolButton(this);
        m_CompileBtn->setIcon(QIcon(":/icons/compile.svg"));
        m_CompileBtn->setObjectName("weaveCompileBtn");
        m_CompileBtn->setToolTip("Compile graph to bytecode (.wbc)");
        addWidget(m_CompileBtn);

        m_SaveBtn = new QToolButton(this);
        m_SaveBtn->setIcon(QIcon(":/icons/save.svg"));
        m_SaveBtn->setObjectName("weaveSaveBtn");
        m_SaveBtn->setToolTip("Save graph");
        addWidget(m_SaveBtn);

        addSeparator();

        m_AddNodeBtn = new QToolButton(this);
        m_AddNodeBtn->setText("Add Node");
        m_AddNodeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        m_AddNodeBtn->setObjectName("weaveAddNodeBtn");
        m_AddNodeBtn->setToolTip("Search and spawn a node (Alt+Space)");
        addWidget(m_AddNodeBtn);

        addSeparator();

        m_ZoomFitBtn = new QToolButton(this);
        m_ZoomFitBtn->setIcon(QIcon(":/icons/zoom_fit.svg"));
        m_ZoomFitBtn->setObjectName("weaveZoomFitBtn");
        m_ZoomFitBtn->setToolTip("Zoom to fit all nodes (F)");
        addWidget(m_ZoomFitBtn);

        m_MinimapBtn = new QToolButton(this);
        m_MinimapBtn->setIcon(QIcon(":/icons/minimap.svg"));
        m_MinimapBtn->setObjectName("weaveMinimapBtn");
        m_MinimapBtn->setCheckable(true);
        m_MinimapBtn->setChecked(true);
        m_MinimapBtn->setToolTip("Toggle minimap");
        addWidget(m_MinimapBtn);

        QWidget* spacer = new QWidget(this);
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        addWidget(spacer);

        connect(m_CompileBtn, &QToolButton::clicked, this, &WeaveToolBar::CompileRequested);
        connect(m_SaveBtn, &QToolButton::clicked, this, &WeaveToolBar::SaveRequested);
        connect(m_AddNodeBtn, &QToolButton::clicked, this, &WeaveToolBar::AddNodeRequested);
        connect(m_ZoomFitBtn, &QToolButton::clicked, this, &WeaveToolBar::ZoomToFitRequested);
        connect(m_MinimapBtn, &QToolButton::toggled, this, &WeaveToolBar::ToggleMinimapRequested);
    }
} // namespace NuEditor::Weave