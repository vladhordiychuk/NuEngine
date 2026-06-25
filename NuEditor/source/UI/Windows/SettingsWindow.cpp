#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QAbstractButton>
#include <QGridLayout>
#include <QMenuBar>
#include <QFile>
#include <QApplication>
#include <QPainter>
#include <QScrollArea>

#include <UI/Windows/SettingsWindow.hpp>
#include <Core/ThemeManager.hpp>

namespace NuEditor
{
    class ThemeCard : public QAbstractButton
    {
    public:
        ThemeCard(EditorTheme theme, const QString& label, const QColor& accent, QWidget* parent = nullptr)
            : QAbstractButton(parent)
            , m_Theme(theme)
            , m_Accent(accent)
        {
            setCheckable(true);
            setText(label);
            setFixedSize(360, 202);
            setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            setCursor(Qt::PointingHandCursor);
            setToolTip(label);
        }

        EditorTheme Theme() const { return m_Theme; }

    protected:
        void paintEvent(QPaintEvent*) override
        {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);

            const bool sel = isChecked();

            p.setPen(QPen(sel ? m_Accent : QColor("#2A2420"), sel ? 1.5 : 0.5));
            p.setBrush(sel ? QColor("#1E1A10") : QColor("#151210"));
            p.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 6, 6);

            QRect tbRect(8, 8, width() - 16, 7);
            p.fillRect(tbRect, QColor("#0F0D0B"));

            QRect sideRect(8, 17, 18, height() - 36);
            QRect vpRect(28, 17, width() - 36, height() - 36);
            p.fillRect(sideRect, QColor("#111009"));
            p.fillRect(vpRect, QColor("#171410"));

            p.fillRect(QRect(8, 24, 2, 5), m_Accent);
            p.fillRect(QRect(11, 25, 12, 3), m_Accent.darker(200));

            p.fillRect(QRect(11, 31, 10, 3), QColor("#1E1A10"));
            p.fillRect(QRect(11, 36, 12, 3), QColor("#1E1A10"));

            p.setPen(sel ? m_Accent : QColor("#6A6458"));
            p.setFont(QFont("Segoe UI", 8, sel ? QFont::Medium : QFont::Normal));
            p.drawText(QRect(8, height() - 18, width() - 24, 14),
                Qt::AlignLeft | Qt::AlignVCenter, text());

            if (sel)
            {
                p.setPen(QPen(m_Accent, 1.5));
                QRect cr(width() - 18, height() - 18, 12, 12);
                p.drawEllipse(cr);
                p.setFont(QFont("Segoe UI", 7, QFont::Bold));
                p.drawText(cr, Qt::AlignCenter, "✓");
            }
        }

        void enterEvent(QEnterEvent*) override { update(); }
        void leaveEvent(QEvent*)      override { update(); }

    private:
        EditorTheme m_Theme;
        QColor m_Accent;
    };

    class ResponsiveThemeGrid : public QWidget
    {
    public:
        explicit ResponsiveThemeGrid(QWidget* parent = nullptr) : QWidget(parent) {}

        void AddCard(ThemeCard* card)
        {
            card->setParent(this);
            m_Cards.append(card);
            UpdateLayout();
        }

    protected:
        void resizeEvent(QResizeEvent* event) override
        {
            UpdateLayout();
            QWidget::resizeEvent(event);
        }

    private:
        static constexpr int kCardW = 360;
        static constexpr int kCardH = 202;
        static constexpr int kSpacing = 12;

        void UpdateLayout()
        {
            const int w = width();

            static constexpr int kSpacingMin = 10;
            static constexpr int kSpacingMax = 128;

            int columns;
            if (w >= 3 * kCardW + 4 * kSpacingMin) columns = 3;
            else if (w >= 2 * kCardW + 3 * kSpacingMin) columns = 2;
            else                                         columns = 1;

            const int leftover = w - columns * kCardW;
            const int spacing = qMin(kSpacingMax, leftover / (columns + 1));

            const int totalCardsW = columns * kCardW + (columns - 1) * spacing;
            const int offsetX = (w - totalCardsW) / 2;

            for (int i = 0; i < m_Cards.size(); ++i)
            {
                const int col = i % columns;
                const int row = i / columns;
                const int x = offsetX + col * (kCardW + spacing);
                const int y = row * (kCardH + spacing);
                m_Cards[i]->setFixedSize(kCardW, kCardH);
                m_Cards[i]->move(x, y);
                m_Cards[i]->show();
            }

            const int rows = (m_Cards.size() + columns - 1) / columns;
            const int totalH = rows * kCardH + (rows - 1) * spacing;
            setMinimumSize(kCardW, totalH);
        }

        QList<ThemeCard*> m_Cards;
    };

    SettingsWindow::SettingsWindow(QWidget* parent)
        : ResizableWindow(parent)
        , m_SelectedTheme(EditorPreferences::Get().GetTheme())
    {
        setMinimumSize(580, 420);
        SetupTitleBar();
        SetupUI();
    }

    void SettingsWindow::SetupTitleBar()
    {
        m_TitleBar = new CustomTitleBar(this);

        m_TitleBar->MenuBar()->hide();

        connect(m_TitleBar, &CustomTitleBar::MinimizeRequested, this, &QWidget::showMinimized);
        connect(m_TitleBar, &CustomTitleBar::MaximizeRequested, this, &ResizableWindow::ToggleCustomMaximize);
        connect(m_TitleBar, &CustomTitleBar::CloseRequested, this, &QWidget::close);
        connect(this, &ResizableWindow::customMaximizeStateChanged,
            this, [this](bool maximized) {
                m_TitleBar->ForceMaxIconState(maximized);
            });

        setMenuWidget(m_TitleBar);
        installMouseTracking(m_TitleBar);
    }

    void SettingsWindow::SetupUI()
    {
        auto* central = new QWidget(this);
        central->setObjectName("settingsCentral");
        setCentralWidget(central);

        auto* root = new QHBoxLayout(central);
        root->setContentsMargins(0, 0, 0, 0);
        root->setSpacing(0);

        m_NavList = new QListWidget(central);
        m_NavList->setObjectName("settingsNavList");
        m_NavList->setFixedWidth(155);
        m_NavList->setFrameShape(QFrame::NoFrame);

        auto addSection = [&](const QString& text) {
            auto* item = new QListWidgetItem(text, m_NavList);
            item->setFlags(Qt::NoItemFlags);
            item->setData(Qt::UserRole, "section");
            };

        auto addItem = [&](const QString& icon, const QString& text) {
            auto* item = new QListWidgetItem(QIcon(":/icons/" + icon), text, m_NavList);
            item->setSizeHint(QSize(0, 32));
            };

        addSection("  GENERAL");
        addItem("palette.svg", "Appearance");
        addItem("keyboard.svg", "Keybindings");
        addItem("layout.svg", "Layout");
        addSection("  EDITOR");
        addItem("performance.svg", "Performance");
        addItem("code.svg", "Scripting");

        m_NavList->setCurrentRow(1);

        m_Pages = new QStackedWidget(central);
        BuildAppearancePage();

        m_Pages->addWidget(new QWidget); // Keybindings
        m_Pages->addWidget(new QWidget); // Layout
        m_Pages->addWidget(new QWidget); // Performance
        m_Pages->addWidget(new QWidget); // Scripting

        connect(m_NavList, &QListWidget::currentRowChanged, this, [this](int row) {
            const QMap<int, int> rowToPage = {
                {1, 0}, {2, 1}, {3, 2}, {5, 3}, {6, 4}
            };

            if (rowToPage.contains(row))
            {
                m_Pages->setCurrentIndex(rowToPage[row]);
            }
            });

        auto* rightCol = new QVBoxLayout;
        rightCol->setContentsMargins(0, 0, 0, 0);
        rightCol->setSpacing(0);
        rightCol->addWidget(m_Pages);

        auto* footerWidget = new QWidget(central);
        footerWidget->setObjectName("settingsFooter");

        auto* footer = new QHBoxLayout(footerWidget);
        footer->setContentsMargins(16, 8, 18, 8);
        footer->addStretch();

        auto* cancelBtn = new QPushButton("Cancel", footerWidget);
        auto* applyBtn = new QPushButton("Apply", footerWidget);
        applyBtn->setObjectName("primaryBtn");

        footer->addWidget(cancelBtn);
        footer->addWidget(applyBtn);
        rightCol->addWidget(footerWidget);

        root->addWidget(m_NavList);
        root->addLayout(rightCol);

        connect(cancelBtn, &QPushButton::clicked, this, &QWidget::close);
        connect(applyBtn, &QPushButton::clicked, this, &SettingsWindow::OnApply);
    }

    void SettingsWindow::BuildAppearancePage()
    {
        auto* page = new QWidget;
        auto* layout = new QVBoxLayout(page);
        layout->setContentsMargins(24, 20, 24, 16);
        layout->setSpacing(14);

        auto* title = new QLabel("Appearance", page);
        title->setObjectName("settingsPageTitle");
        layout->addWidget(title);

        auto* sub = new QLabel("Customize colors and interface scale", page);
        sub->setObjectName("settingsPageSub");
        layout->addWidget(sub);

        auto* sep = new QFrame(page);
        sep->setFrameShape(QFrame::HLine);
        sep->setObjectName("settingsSeparator");
        layout->addWidget(sep);

        auto* themeLabel = new QLabel("COLOR THEME", page);
        themeLabel->setObjectName("settingsGroupLabel");
        layout->addWidget(themeLabel);

        struct ThemeDef
        {
            EditorTheme theme;
            QString label;
            QColor accent;
        };

        const QList<ThemeDef> themes = {
            { EditorTheme::WarmEmber,    "Warm Ember",    QColor("#B08040") },
            { EditorTheme::VolcanicAsh,  "Volcanic Ash",  QColor("#D04040") },
            { EditorTheme::ArcticSlate,  "Arctic Slate",  QColor("#4AC0E0") },
            { EditorTheme::ForestDepth,  "Forest Depth",  QColor("#8AE070") },
            { EditorTheme::DuskPurple,   "Dusk Purple",   QColor("#C0A8F0") },
            { EditorTheme::MidnightRose, "Midnight Rose", QColor("#F06888") },
        };

        m_ThemeGroup = new QButtonGroup(page);
        m_ThemeGroup->setExclusive(true);

        auto* responsiveGrid = new ResponsiveThemeGrid(page);

        for (int i = 0; i < themes.size(); ++i)
        {
            auto* card = new ThemeCard(themes[i].theme, themes[i].label, themes[i].accent, responsiveGrid);

            if (themes[i].theme == m_SelectedTheme)
            {
                card->setChecked(true);
            }

            m_ThemeGroup->addButton(card);

            responsiveGrid->AddCard(card);

            connect(card, &QAbstractButton::toggled, this, [this, card](bool checked) {
                    if (checked)
                    {
                        m_SelectedTheme = card->Theme();
                    }
                });
        }

        auto* scroll = new QScrollArea(page);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scroll->setWidget(responsiveGrid);

        layout->addWidget(scroll);
        m_Pages->addWidget(page);
    }

    void SettingsWindow::OnApply()
    {
        const bool changed =
            (m_SelectedTheme != EditorPreferences::Get().GetTheme());

        if (changed)
        {
            EditorPreferences::Get().SetTheme(m_SelectedTheme);
            EditorPreferences::Get().Save();

            Core::ThemeManager::Get().Apply(m_SelectedTheme);
        }

        close();
    }
} // namespace NuEditor