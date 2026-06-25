#pragma once

#include <QToolBar>
#include <QToolButton>
#include <QLabel>

namespace NuEditor::Weave
{
    class WeaveToolBar : public QToolBar
    {
        Q_OBJECT
    public:
        explicit WeaveToolBar(QWidget* parent = nullptr);

    signals:
        void CompileRequested();
        void SaveRequested();
        void AddNodeRequested();
        void ZoomToFitRequested();
        void ToggleMinimapRequested(bool visible);

    private:
        QToolButton* m_CompileBtn = nullptr;
        QToolButton* m_SaveBtn = nullptr;
        QToolButton* m_AddNodeBtn = nullptr;
        QToolButton* m_ZoomFitBtn = nullptr;
        QToolButton* m_MinimapBtn = nullptr;
        QLabel* m_GraphLabel = nullptr;

        void SetupUI();
    };
} // namespace NuEditor::Weave