#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>

#include <UI/Panels/ConsolePanel.hpp>

namespace NuEditor
{
    ConsolePanel::ConsolePanel(QWidget* parent)
        : QWidget(parent)
    {
        SetupUI();
        connect(this, &ConsolePanel::LogReceived, this, &ConsolePanel::OnLogReceived, Qt::QueuedConnection);
    }

    ConsolePanel::~ConsolePanel()
    {
        DisconnectFromLogger();
    }

    void ConsolePanel::SetupUI()
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(4, 4, 4, 4);
        layout->setSpacing(4);

        QWidget* toolbar = new QWidget(this);
        QHBoxLayout* tbLayout = new QHBoxLayout(toolbar);
        tbLayout->setContentsMargins(0, 0, 0, 0);
        tbLayout->setSpacing(4);

        m_ClearBtn = new QToolButton(toolbar);
        m_ClearBtn->setText("Clear");
        m_ClearBtn->setObjectName("consoleClearBtn");

        m_InfoBtn = new QToolButton(toolbar);
        m_InfoBtn->setText("Info");
        m_InfoBtn->setCheckable(true);
        m_InfoBtn->setChecked(true);
        m_InfoBtn->setObjectName("consoleInfoBtn");

        m_WarnBtn = new QToolButton(toolbar);
        m_WarnBtn->setText("Warning");
        m_WarnBtn->setCheckable(true);
        m_WarnBtn->setChecked(true);
        m_WarnBtn->setObjectName("consoleWarnBtn");

        m_ErrorBtn = new QToolButton(toolbar);
        m_ErrorBtn->setText("Error");
        m_ErrorBtn->setCheckable(true);
        m_ErrorBtn->setChecked(true);
        m_ErrorBtn->setObjectName("consoleErrorBtn");

        tbLayout->addWidget(m_ClearBtn);
        tbLayout->addSpacing(8);
        tbLayout->addWidget(m_InfoBtn);
        tbLayout->addWidget(m_WarnBtn);
        tbLayout->addWidget(m_ErrorBtn);
        tbLayout->addStretch();

        m_LogList = new QListWidget(this);
        m_LogList->setObjectName("consoleList");
        m_LogList->setWordWrap(true);
        m_LogList->setSelectionMode(QAbstractItemView::SingleSelection);

        layout->addWidget(toolbar);
        layout->addWidget(m_LogList);

        connect(m_ClearBtn, &QToolButton::clicked, this, &ConsolePanel::OnClearClicked);
        connect(m_InfoBtn, &QToolButton::toggled, this, &ConsolePanel::OnFilterToggled);
        connect(m_WarnBtn, &QToolButton::toggled, this, &ConsolePanel::OnFilterToggled);
        connect(m_ErrorBtn, &QToolButton::toggled, this, &ConsolePanel::OnFilterToggled);
    }

    void ConsolePanel::ConnectToLogger()
    {
        NuEngine::Core::Logger::SetEditorCallback(
            [this](NuEngine::Core::LogLevel level, const std::string& msg, const std::string& ts)
            {
                int editorLevel;
                switch (level)
                {
                    case NuEngine::Core::LogLevel::Warning:
                    case NuEngine::Core::LogLevel::Critical:
                        editorLevel = static_cast<int>(LogLevel::Warning); 
                        break;
                    case NuEngine::Core::LogLevel::Error:
                        editorLevel = static_cast<int>(LogLevel::Error);   
                        break;
                    default:
                        editorLevel = static_cast<int>(LogLevel::Info);
                }
                emit LogReceived(editorLevel, QString::fromStdString(msg), QString::fromStdString(ts));
            });
    }

    void ConsolePanel::DisconnectFromLogger()
    {
        NuEngine::Core::Logger::ClearEditorCallback();
    }

    void ConsolePanel::OnLogReceived(int level, QString message, QString timestamp)
    {
        LogEntry entry;
        entry.level = static_cast<LogLevel>(level);
        entry.message = message;
        entry.timestamp = timestamp;
        m_Entries.append(entry);

        bool visible = (entry.level == LogLevel::Info && m_ShowInfo)
            || (entry.level == LogLevel::Warning && m_ShowWarn)
            || (entry.level == LogLevel::Error && m_ShowError);
        if (visible)
        {
            AddItem(entry);
            m_LogList->scrollToBottom();
        }
    }

    void ConsolePanel::AddItem(const LogEntry& entry)
    {
        QString prefix;
        switch (entry.level)
        {
            case LogLevel::Info:    
                prefix = "[INFO]";    
                break;
            case LogLevel::Warning: 
                prefix = "[WARNING]"; 
                break;
            case LogLevel::Error:   
                prefix = "[ERROR]";   
                break;
        }
        QString text = QString("[%1]  %2  %3").arg(entry.timestamp, prefix, entry.message);
        QListWidgetItem* item = new QListWidgetItem(text, m_LogList);
        item->setForeground(LevelColor(entry.level));
    }

    void ConsolePanel::RebuildList()
    {
        m_LogList->clear();
        for (const auto& entry : m_Entries)
        {
            bool show = (entry.level == LogLevel::Info && m_ShowInfo)
                || (entry.level == LogLevel::Warning && m_ShowWarn)
                || (entry.level == LogLevel::Error && m_ShowError);
            if (show)
            {
                AddItem(entry);
            }
        }
    }

    void ConsolePanel::OnClearClicked()
    {
        m_Entries.clear();
        m_LogList->clear();
    }

    void ConsolePanel::OnFilterToggled()
    {
        m_ShowInfo = m_InfoBtn->isChecked();
        m_ShowWarn = m_WarnBtn->isChecked();
        m_ShowError = m_ErrorBtn->isChecked();
        RebuildList();
    }

    QColor ConsolePanel::LevelColor(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Info:   
                return QColor("#d4d4d4");
            case LogLevel::Warning: 
                return QColor("#e5c07b");
            case LogLevel::Error:   
                return QColor("#e06c75");
            default:                
                return QColor("#d4d4d4");
        }
    }
}