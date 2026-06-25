#pragma once

#include <App/ResizableWindow.hpp>
#include <Ui/Panels/CustomTitleBar.hpp>
#include <Weave/WeaveToolBar.hpp>
#include <Weave/WeaveGraphView.hpp>
#include <Weave/WeaveGraphScene.hpp>
#include <Weave/Utils/WeaveCommands.hpp>

#include <QAction>
#include <QCloseEvent>

namespace NuEditor::Weave
{
	class WeaveWindow : public ResizableWindow
	{
		Q_OBJECT
	public:
		explicit WeaveWindow(QWidget* parent = nullptr);

		void LoadGraph(const QString& path);

	protected:
		void closeEvent(QCloseEvent* event) override;

		void OnResizeAnimationStart() override;
		void OnResizeAnimationEnd() override;

	private slots:
		void OnCompile();
		void OnSave();
		void OnSaveAs();
		void OnOpen();
		void OnAddNode();
		void OnZoomToFit();
		void OnToggleMinimap(bool visible);
		void OnGraphChanged();

		void OnUndo();
		void OnRedo();
		void OnCopy();
		void OnCut();
		void OnPaste();
		void OnSelectAll();

		void OnConnectionRequested(WeaveConnection conn);
		void OnNodeRemoveRequested(int nodeId);
		void OnDisconnectRequested(WeaveConnection conn);

	private:
		void SetupTitleBar();
		void SetupUI();

		CustomTitleBar* m_TitleBar = nullptr;
		WeaveToolBar* m_ToolBar = nullptr;
		WeaveGraphView* m_GraphView = nullptr;
		WeaveGraphScene* m_Scene = nullptr;

		WeaveCommandStack* m_CmdStack = nullptr;

		QString m_CurrentFilePath;
		bool m_IsDirty = false;

		QAction* m_OpenAction = nullptr;
		QAction* m_SaveAction = nullptr;
		QAction* m_SaveAsAction = nullptr;

		QAction* m_UndoAction = nullptr;
		QAction* m_RedoAction = nullptr;

		int m_PasteCounter = 1;
	};
} // namespace NuEditor::Weave