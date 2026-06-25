#pragma once

#include <QLineEdit>
#include <QIcon>
#include <QString>
#include <QPaintEvent>

namespace NuEditor
{
    class SearchLineEdit : public QLineEdit
    {
        Q_OBJECT
    public:
        explicit SearchLineEdit(QWidget* parent = nullptr);

        void setCustomPlaceholder(const QString& text);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        QIcon m_SearchIcon;
    };
} // namespace NuEditor