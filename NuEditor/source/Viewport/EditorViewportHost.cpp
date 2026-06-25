#include <Viewport/EditorViewportHost.hpp>

namespace NuEditor
{
	EditorViewportHost::EditorViewportHost(EditorRuntime* runtime, QWidget* parent)
		: QWidget(parent), m_Runtime(runtime)
	{
		auto* layout = new QVBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);

		m_GLWidget = new NuOpenGLWidget(runtime->GetApp(), this);
		layout->addWidget(m_GLWidget);

		connect(m_GLWidget, &NuOpenGLWidget::glInitialized, this, [this]() {
			if (m_Runtime->GetApp())
			{
				m_GLWidget->InitializeApp();
			}

			emit glInitialized();
			});

		connect(m_Runtime, &EditorRuntime::FrameReady, this, [this](float) {
			m_GLWidget->update();
			});

		connect(m_Runtime, &EditorRuntime::DllLoaded, this, [this]() {
			m_GLWidget->SetApp(m_Runtime->GetApp());

			if (m_GLWidget->IsGlInitialized())
			{
				m_GLWidget->InitializeApp();
			}
			});

		connect(m_Runtime, &EditorRuntime::AppResetRequired, this, [this]() {
			m_GLWidget->SetApp(m_Runtime->GetApp());

			if (m_GLWidget->IsGlInitialized())
			{
				m_GLWidget->InitializeApp();
			}
			});
	}
}