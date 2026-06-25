#pragma once

#include <App/ResizableWindow.hpp>
#include <Runtime/EditorRuntime.hpp>
#include <Viewport/EditorViewportHost.hpp>
#include <UI/EditorUI.hpp>

#include <Weave/WeaveWindow.hpp>

namespace NuEditor
{
    class EditorWindow : public ResizableWindow
    {
        Q_OBJECT
    public:
        explicit EditorWindow(QWidget* parent = nullptr);
        ~EditorWindow();

    protected:
        void closeEvent(QCloseEvent* event) override;

    private slots:
        void OnLoadDll();
        void OnNodeEditor();

    private:
        EditorRuntime* m_Runtime = nullptr;
        EditorViewportHost* m_Viewport = nullptr;
        EditorUI* m_UI = nullptr;
        Weave::WeaveWindow* m_WeaveWindow = nullptr;
    };
}