#pragma once

#include <QWidget>
#include <QVBoxLayout>

#include <Viewport/NuOpenGLWidget.hpp>
#include <Runtime/EditorRuntime.hpp>

namespace NuEditor
{
	class EditorViewportHost : public QWidget
	{
		Q_OBJECT
	public:
		explicit EditorViewportHost(EditorRuntime* runtime, QWidget* parent = nullptr);

		[[nodiscard]] NuOpenGLWidget* GLWidget() const { return m_GLWidget; }

	signals:
		void glInitialized();

	private:
		NuOpenGLWidget* m_GLWidget = nullptr;
		EditorRuntime* m_Runtime = nullptr;
	};
}