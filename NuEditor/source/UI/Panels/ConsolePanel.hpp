#pragma once

#include <QWidget>
#include <QListWidget>
#include <QToolButton>

#include <NuEngine/Core/Logging/Logger.hpp>

namespace NuEditor
{
    enum class LogLevel 
    { 
        Info, 
        Warning, 
        Error 
    };

    struct LogEntry
    {
        LogLevel level;
        QString  message;
        QString  timestamp;
    };

    class ConsolePanel : public QWidget
    {
        Q_OBJECT
    public:
        explicit ConsolePanel(QWidget* parent = nullptr);
        ~ConsolePanel();

        void ConnectToLogger();
        void DisconnectFromLogger();

    signals:
        void LogReceived(int level, QString message, QString timestamp);

    private slots:
        void OnClearClicked();
        void OnFilterToggled();
        void OnLogReceived(int level, QString message, QString timestamp);

    private:
        QListWidget* m_LogList = nullptr;
        QToolButton* m_ClearBtn = nullptr;
        QToolButton* m_InfoBtn = nullptr;
        QToolButton* m_WarnBtn = nullptr;
        QToolButton* m_ErrorBtn = nullptr;

        QList<LogEntry> m_Entries;
        bool m_ShowInfo = true;
        bool m_ShowWarn = true;
        bool m_ShowError = true;

        void   SetupUI();
        void   AddItem(const LogEntry& entry);
        void   RebuildList();
        [[nodiscard]] QColor LevelColor(LogLevel level);
    };
} // namespace NuEditor