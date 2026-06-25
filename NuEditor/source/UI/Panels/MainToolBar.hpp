#pragma once

#include <QToolBar>
#include <QToolButton>
#include <QPushButton>

#include <UI/Widgets/SearchLineEdit.hpp>
#include <Core/EditorState.hpp>

namespace NuEditor
{
    enum class GizmoMode
    {
        Select,
        Translate,
        Rotate,
        Scale
    };

    class MainToolBar : public QToolBar
    {
        Q_OBJECT
    public:
        explicit MainToolBar(QWidget* parent = nullptr);
        void SetEditorState(EditorState state);

    signals:
        void SettingsRequested();
        void NewSceneRequested();
        void OpenSceneRequested();
        void SaveSceneRequested();
        void ReloadRequested();

        void PlayRequested();
        void PauseRequested();
        void StopRequested();

        void GizmoModeChanged(GizmoMode mode);

    private:
        void SetupUI();

        QToolButton* m_NewSceneBtn = nullptr;
        QToolButton* m_OpenSceneBtn = nullptr;
        QToolButton* m_SaveBtn = nullptr;
        QToolButton* m_ReloadBtn = nullptr;

        QToolButton* m_SelectBtn = nullptr;
        QToolButton* m_TranslateBtn = nullptr;
        QToolButton* m_RotateBtn = nullptr;
        QToolButton* m_ScaleBtn = nullptr;

        QToolButton* m_PlayBtn = nullptr;
        QToolButton* m_PauseBtn = nullptr;
        QToolButton* m_StopBtn = nullptr;

        SearchLineEdit* m_SearchBar = nullptr;

        QToolButton* m_SettingsBtn = nullptr;
    };
} // namespace NuEditor