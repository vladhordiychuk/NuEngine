#include <QPainter>

#include <UI/Widgets/SearchLineEdit.hpp>

namespace NuEditor
{
    SearchLineEdit::SearchLineEdit(QWidget* parent)
        : QLineEdit(parent)
    {
        m_SearchIcon = QIcon(":/icons/search.svg");

        setTextMargins(26, 0, 0, 0);
    }

    void SearchLineEdit::setCustomPlaceholder(const QString& text)
    {
        setPlaceholderText(text);
    }

    void SearchLineEdit::paintEvent(QPaintEvent* event)
    {
        QLineEdit::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int iconSize = 16;
        int paddingX = 6;

        QRect iconRect(paddingX, (height() - iconSize) / 2, iconSize, iconSize);
        m_SearchIcon.paint(&painter, iconRect);
    }
} // namespace NuEditor