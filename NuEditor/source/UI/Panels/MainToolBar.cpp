#include <QButtonGroup>
#include <QHBoxLayout>
#include <QWidget>
#include <QIcon>

#include <UI/Panels/MainToolBar.hpp>

namespace NuEditor
{
    MainToolBar::MainToolBar(QWidget* parent)
        : QToolBar("Main Editor Toolbar", parent)
    {
        setMovable(false);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        SetupUI();
    }

    void MainToolBar::SetEditorState(EditorState state)
    {
        switch (state)
        {
            case EditorState::Playing:
                m_PlayBtn->setEnabled(false);
                m_PauseBtn->setEnabled(true);
                m_StopBtn->setEnabled(true);
                break;
            case EditorState::Paused:
                m_PlayBtn->setEnabled(true);
                m_PauseBtn->setEnabled(false);
                m_StopBtn->setEnabled(true);
                break;
            case EditorState::Stopped:
                m_PlayBtn->setEnabled(true);
                m_PauseBtn->setEnabled(false);
                m_StopBtn->setEnabled(false);
                break;
        }
    }

    void MainToolBar::SetupUI()
    {
        m_NewSceneBtn = new QToolButton(this);
        m_NewSceneBtn->setObjectName("newSceneBtn");
        m_NewSceneBtn->setIcon(QIcon(":/icons/new_scene.svg"));
        
        m_OpenSceneBtn = new QToolButton(this);
        m_OpenSceneBtn->setObjectName("openSceneBtn");
        m_OpenSceneBtn->setIcon(QIcon(":/icons/open_scene.svg"));

        m_SaveBtn = new QToolButton(this);
        m_SaveBtn->setObjectName("saveBtn");
        m_SaveBtn->setIcon(QIcon(":/icons/save.svg"));

        m_ReloadBtn = new QToolButton(this);
        m_ReloadBtn->setObjectName("reloadBtn");
        m_ReloadBtn->setIcon(QIcon(":/icons/reload.svg"));

        addWidget(m_NewSceneBtn);
        addWidget(m_OpenSceneBtn);

        addSeparator();

        addWidget(m_SaveBtn);
        addWidget(m_ReloadBtn);

        addSeparator();

        m_SelectBtn = new QToolButton(this);
        m_SelectBtn->setObjectName("selectBtn");
        m_SelectBtn->setIcon(QIcon(":/icons/select.svg"));
        m_SelectBtn->setToolTip("Select  [Q]");
        m_SelectBtn->setCheckable(true);
        m_SelectBtn->setChecked(true);

        m_TranslateBtn = new QToolButton(this);
        m_TranslateBtn->setObjectName("translateBtn");
        m_TranslateBtn->setIcon(QIcon(":/icons/translate.svg"));
        m_TranslateBtn->setToolTip("Translate  [W]");
        m_TranslateBtn->setCheckable(true);
        m_TranslateBtn->setChecked(false);

        m_RotateBtn = new QToolButton(this);
        m_RotateBtn->setObjectName("rotateBtn");
        m_RotateBtn->setIcon(QIcon(":/icons/rotate.svg"));
        m_RotateBtn->setToolTip("Rotate  [E]");
        m_RotateBtn->setCheckable(true);

        m_ScaleBtn = new QToolButton(this);
        m_ScaleBtn->setObjectName("scaleBtn");
        m_ScaleBtn->setIcon(QIcon(":/icons/scale.svg"));
        m_ScaleBtn->setToolTip("Scale  [R]");
        m_ScaleBtn->setCheckable(true);

        QButtonGroup* gizmoGroup = new QButtonGroup(this);
        gizmoGroup->setExclusive(true);
        gizmoGroup->addButton(m_SelectBtn);
        gizmoGroup->addButton(m_TranslateBtn);
        gizmoGroup->addButton(m_RotateBtn);
        gizmoGroup->addButton(m_ScaleBtn);

        addWidget(m_SelectBtn);
        addWidget(m_TranslateBtn);
        addWidget(m_RotateBtn);
        addWidget(m_ScaleBtn);

        addSeparator();

        m_PlayBtn = new QToolButton(this);
        m_PlayBtn->setIcon(QIcon(":/icons/play.svg"));
        m_PlayBtn->setObjectName("playBtn");
        m_PlayBtn->setToolTip("Play  [F5]");

        m_PauseBtn = new QToolButton(this);
        m_PauseBtn->setIcon(QIcon(":/icons/pause.svg"));
        m_PauseBtn->setObjectName("pauseBtn");
        m_PauseBtn->setToolTip("Pause");
        m_PauseBtn->setEnabled(false);

        m_StopBtn = new QToolButton(this);
        m_StopBtn->setIcon(QIcon(":/icons/stop.svg"));
        m_StopBtn->setObjectName("stopBtn");
        m_StopBtn->setToolTip("Stop  [F5]");
        m_StopBtn->setEnabled(false);

        addWidget(m_PlayBtn);
        addWidget(m_PauseBtn);
        addWidget(m_StopBtn);

        addSeparator();

        m_SearchBar = new SearchLineEdit(this);
        m_SearchBar->setObjectName("searchBar");
        m_SearchBar->setMinimumWidth(280);
        m_SearchBar->setMaximumWidth(400);
        m_SearchBar->setCustomPlaceholder("Search in project...");
        addWidget(m_SearchBar);

        QWidget* spacer = new QWidget(this);
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        spacer->setObjectName("mainToolBarSpacer");
        addWidget(spacer);

        m_SettingsBtn = new QToolButton(this);
        m_SettingsBtn->setObjectName("settingsBtn");
        m_SettingsBtn->setIcon(QIcon(":/icons/settings.svg"));
        addWidget(m_SettingsBtn);

        connect(m_PlayBtn, &QToolButton::clicked, this, &MainToolBar::PlayRequested);
        connect(m_PauseBtn, &QToolButton::clicked, this, &MainToolBar::PauseRequested);
        connect(m_StopBtn, &QToolButton::clicked, this, &MainToolBar::StopRequested);

        connect(gizmoGroup, &QButtonGroup::buttonClicked,
            this, [this](QAbstractButton* btn) {
                if (btn == m_SelectBtn)
                {
                    emit GizmoModeChanged(GizmoMode::Select);
                }
                else if (btn == m_TranslateBtn)
                {
                    emit GizmoModeChanged(GizmoMode::Translate);
                }
                else if (btn == m_RotateBtn)
                {
                    emit GizmoModeChanged(GizmoMode::Rotate);
                }
                else
                {
                    emit GizmoModeChanged(GizmoMode::Scale);
                }
            });

        connect(m_SettingsBtn, &QToolButton::clicked, this, &MainToolBar::SettingsRequested);

        connect(m_NewSceneBtn, &QToolButton::clicked, this, &MainToolBar::NewSceneRequested);
        connect(m_OpenSceneBtn, &QToolButton::clicked, this, &MainToolBar::OpenSceneRequested);
        connect(m_SaveBtn, &QToolButton::clicked, this, &MainToolBar::SaveSceneRequested);
        connect(m_ReloadBtn, &QToolButton::clicked, this, &MainToolBar::ReloadRequested);
    }
} // namespace NuEditor