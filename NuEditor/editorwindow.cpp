#include "EditorWindow.hpp"
#include "./ui_editorwindow.h"
#include <QVBoxLayout>
#include <QDockWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QStatusBar>
#include <QLabel>
#include <NuEngine/Core/Timer/Time.hpp>

EditorWindow::EditorWindow(NuEngine::Runtime::Application* engineApp, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
    , m_EngineApp(engineApp)
{
    ui->setupUi(this);

    QWidget* central = ui->centralwidget;
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    m_Viewport = new NuOpenGLWidget(m_EngineApp, central);
    layout->addWidget(m_Viewport);

    QDockWidget* inspectorDock = new QDockWidget("Inspector", this);
    inspectorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* dockContents = new QWidget();
    QVBoxLayout* dockLayout = new QVBoxLayout(dockContents);

    QPushButton* colorBtn = new QPushButton("Change Clear Color", dockContents);
    dockLayout->addWidget(colorBtn);
    dockLayout->addStretch();

    inspectorDock->setWidget(dockContents);
    this->addDockWidget(Qt::RightDockWidgetArea, inspectorDock);

    connect(colorBtn, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(Qt::gray, this, "Select Background");
        if (color.isValid()) {
            m_EngineApp->SetClearColor(color.redF(), color.greenF(), color.blueF(), 1.0f);
        }
        });

    m_FpsLabel = new QLabel("FPS: 0 | Frame Time: 0.00 ms", this);
    m_FpsLabel->setStyleSheet("padding: 2px 8px; color: #00FF00;");
    statusBar()->addPermanentWidget(m_FpsLabel);

    connect(&m_Timer, &QTimer::timeout, this, &EditorWindow::UpdateGameLoop);
    m_Timer.start(0);
}

EditorWindow::~EditorWindow()
{
    m_Timer.stop();
    delete ui;
}

void EditorWindow::UpdateGameLoop()
{
    NuEngine::Core::Time::Update();
    float dt = NuEngine::Core::Time::GetDeltaTime();

    m_FrameCount++;
    m_FpsTimer += dt;

    if (m_FpsTimer >= 1.0f)
    {
        float fps = m_FrameCount / m_FpsTimer;
        float msPerFrame = 1000.0f / fps;

        QString color;
        if (fps >= 60) color = "#00FF00";
        else if (fps >= 30) color = "#FFAA00";
        else color = "#FF0000";

        m_FpsLabel->setStyleSheet(QString("padding: 2px 8px; color: %1; font-weight: bold;").arg(color));

        m_FpsLabel->setText(QString("FPS: %1 | Frame Time: %2 ms")
            .arg((int)fps)
            .arg(msPerFrame, 0, 'f', 2));

        m_FrameCount = 0;
        m_FpsTimer = 0.0f;
    }

    if (m_EngineApp)
    {
        m_EngineApp->UpdateFrame(dt);
    }

    m_Viewport->update();
}