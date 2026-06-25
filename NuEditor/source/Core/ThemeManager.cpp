#include <Core/ThemeManager.hpp>
#include <QApplication>
#include <QFile>
#include <QTextStream>

namespace NuEditor::Core
{
    ThemeManager::ThemeManager()
    {
        m_Palette = MakeWarmEmber();
    }

    void ThemeManager::Apply(EditorTheme theme)
    {
        m_Current = theme;
        BuildPalette(theme);
        ApplyQss(theme);
        emit ThemeChanged();
    }

    void ThemeManager::ApplyCurrent()
    {
        Apply(EditorPreferences::Get().GetTheme());
    }

    QString ThemeManager::QssPath(EditorTheme theme)
    {
        switch (theme)
        {
        case EditorTheme::VolcanicAsh:  return ":/themes/volcanic_ash.qss";
        case EditorTheme::ArcticSlate:  return ":/themes/arctic_slate.qss";
        case EditorTheme::ForestDepth:  return ":/themes/forest_depth.qss";
        case EditorTheme::DuskPurple:   return ":/themes/dusk_purple.qss";
        case EditorTheme::MidnightRose: return ":/themes/midnight_rose.qss";
        default:                        return ":/themes/warm_ember.qss";
        }
    }

    void ThemeManager::ApplyQss(EditorTheme theme)
    {
        auto readFile = [](const QString& path) -> QString {
            QFile f(path);
            if (!f.open(QFile::ReadOnly | QFile::Text))
            {
                qWarning("ThemeManager: cannot open %s", qPrintable(path));
                return {};
            }
            return QTextStream(&f).readAll();
            };

        QString css = readFile(":/themes/base.qss");

        QString themeCss = readFile(QssPath(theme));
        if (themeCss.isEmpty())
        {
            qWarning("ThemeManager: theme file missing, falling back to WarmEmber");
            themeCss = readFile(QssPath(EditorTheme::WarmEmber));
        }

        css += themeCss;
        qApp->setStyleSheet(css);
    }

    void ThemeManager::BuildPalette(EditorTheme theme)
    {
        switch (theme)
        {
        case EditorTheme::VolcanicAsh:  m_Palette = MakeVolcanicAsh();  break;
        case EditorTheme::ArcticSlate:  m_Palette = MakeArcticSlate();  break;
        case EditorTheme::ForestDepth:  m_Palette = MakeForestDepth();  break;
        case EditorTheme::DuskPurple:   m_Palette = MakeDuskPurple();   break;
        case EditorTheme::MidnightRose: m_Palette = MakeMidnightRose(); break;
        default:                        m_Palette = MakeWarmEmber();     break;
        }
    }

    ThemePalette ThemeManager::MakeWarmEmber()
    {
        ThemePalette p;
        p.Background = QColor(0x1A, 0x16, 0x12);
        p.BgAlt = QColor(0x14, 0x12, 0x10);
        p.BgMid = QColor(0x17, 0x14, 0x12);
        p.Surface = QColor(0x22, 0x1E, 0x16);
        p.SurfaceAlt = QColor(0x1E, 0x1A, 0x12);
        p.Border = QColor(0x2A, 0x24, 0x20);
        p.BorderHover = QColor(0x3A, 0x34, 0x28);
        p.Accent = QColor(0xB0, 0x80, 0x40);
        p.AccentHover = QColor(0xC8, 0x98, 0x48);
        p.AccentDim = QColor(0x7A, 0x58, 0x28);
        p.AccentBg = QColor(0x2A, 0x20, 0x10);
        p.Text = QColor(0xC8, 0xBF, 0xB0);
        p.TextMuted = QColor(0x7A, 0x70, 0x60);
        p.TextOff = QColor(0x3A, 0x34, 0x28);
        p.GridLine = QColor(0x30, 0x28, 0x22);
        p.GridDot = QColor(0x40, 0x34, 0x2A);
        p.MinimapBg = QColor(0x14, 0x10, 0x0C, 200);
        p.MinimapBorder = QColor(0x4A, 0x3C, 0x30);
        p.MinimapView = QColor(0xB0, 0x80, 0x40, 40);
        p.NodeBg = QColor(0x2A, 0x24, 0x1E);
        p.NodeBorder = QColor(0x55, 0x48, 0x3C);
        p.NodeBorderSelected = QColor(0xC8, 0x98, 0x48);
        p.PinBg = QColor(0x1C, 0x16, 0x12);
        p.WirePending = QColor(0xE0, 0xA8, 0x50);
        p.ValueBoxBg = QColor(0x18, 0x14, 0x10);
        p.ValueBoxBorder = QColor(0x4A, 0x3C, 0x30);
        p.ValueBoxActive = QColor(0xB0, 0x80, 0x40);
        p.Play = QColor(0x60, 0x9A, 0x60);
        p.Pause = QColor(0xC8, 0x98, 0x48);
        p.Stop = QColor(0xB8, 0x4A, 0x3A);

        return p;
    }

    ThemePalette ThemeManager::MakeArcticSlate()
    {
        ThemePalette p;
        p.Background = QColor(0x12, 0x18, 0x1E);
        p.BgAlt = QColor(0x0E, 0x12, 0x18);
        p.BgMid = QColor(0x10, 0x15, 0x1A);
        p.Surface = QColor(0x18, 0x22, 0x2C);
        p.SurfaceAlt = QColor(0x14, 0x1E, 0x28);
        p.Border = QColor(0x20, 0x2E, 0x3C);
        p.BorderHover = QColor(0x2C, 0x3E, 0x50);
        p.Accent = QColor(0x4A, 0xC0, 0xE0);
        p.AccentHover = QColor(0x60, 0xD4, 0xF0);
        p.AccentDim = QColor(0x28, 0x80, 0xA0);
        p.AccentBg = QColor(0x10, 0x28, 0x38);
        p.Text = QColor(0xC0, 0xD4, 0xE0);
        p.TextMuted = QColor(0x60, 0x80, 0x98);
        p.TextOff = QColor(0x2C, 0x3E, 0x50);
        p.GridLine = QColor(0x1E, 0x2A, 0x36);
        p.GridDot = QColor(0x28, 0x38, 0x48);
        p.MinimapBg = QColor(0x0C, 0x10, 0x16, 200);
        p.MinimapBorder = QColor(0x30, 0x44, 0x58);
        p.MinimapView = QColor(0x4A, 0xC0, 0xE0, 40);
        p.NodeBg = QColor(0x18, 0x22, 0x2C);
        p.NodeBorder = QColor(0x30, 0x44, 0x58);
        p.NodeBorderSelected = QColor(0x4A, 0xC0, 0xE0);
        p.PinBg = QColor(0x10, 0x18, 0x22);
        p.WirePending = QColor(0x4A, 0xC0, 0xE0);
        p.ValueBoxBg = QColor(0x0E, 0x14, 0x1C);
        p.ValueBoxBorder = QColor(0x30, 0x44, 0x58);
        p.ValueBoxActive = QColor(0x4A, 0xC0, 0xE0);
        p.Play = QColor(0x40, 0xB0, 0x80);
        p.Pause = QColor(0x4A, 0xC0, 0xE0);
        p.Stop = QColor(0xC0, 0x50, 0x50);
        return p;
    }

    ThemePalette ThemeManager::MakeVolcanicAsh()
    {
        ThemePalette p = MakeWarmEmber();
        p.Background = QColor(0x18, 0x12, 0x12);
        p.BgAlt = QColor(0x12, 0x0E, 0x0E);
        p.BgMid = QColor(0x15, 0x10, 0x10);
        p.Surface = QColor(0x22, 0x16, 0x16);
        p.SurfaceAlt = QColor(0x1E, 0x12, 0x12);
        p.Border = QColor(0x2E, 0x20, 0x20);
        p.BorderHover = QColor(0x3E, 0x2C, 0x2C);
        p.Accent = QColor(0xD0, 0x40, 0x40);
        p.AccentHover = QColor(0xE8, 0x58, 0x58);
        p.AccentDim = QColor(0x90, 0x20, 0x20);
        p.AccentBg = QColor(0x2C, 0x10, 0x10);
        p.NodeBorderSelected = QColor(0xD0, 0x40, 0x40);
        p.WirePending = QColor(0xFF, 0x80, 0x40);
        p.ValueBoxActive = QColor(0xD0, 0x40, 0x40);
        p.Stop = QColor(0xD0, 0x40, 0x40);
        return p;
    }

    ThemePalette ThemeManager::MakeForestDepth()
    {
        ThemePalette p = MakeWarmEmber();
        p.Background = QColor(0x10, 0x18, 0x12);
        p.BgAlt = QColor(0x0C, 0x12, 0x0E);
        p.BgMid = QColor(0x0E, 0x15, 0x10);
        p.Surface = QColor(0x16, 0x22, 0x18);
        p.SurfaceAlt = QColor(0x12, 0x1E, 0x14);
        p.Border = QColor(0x20, 0x2E, 0x22);
        p.BorderHover = QColor(0x2C, 0x3E, 0x2E);
        p.Accent = QColor(0x8A, 0xE0, 0x70);
        p.AccentHover = QColor(0xA0, 0xF0, 0x88);
        p.AccentDim = QColor(0x50, 0x90, 0x40);
        p.AccentBg = QColor(0x14, 0x28, 0x10);
        p.Text = QColor(0xC0, 0xD4, 0xB8);
        p.TextMuted = QColor(0x60, 0x88, 0x60);
        p.NodeBorderSelected = QColor(0x8A, 0xE0, 0x70);
        p.WirePending = QColor(0xA0, 0xFF, 0x60);
        p.ValueBoxActive = QColor(0x8A, 0xE0, 0x70);
        return p;
    }

    ThemePalette ThemeManager::MakeDuskPurple()
    {
        ThemePalette p = MakeWarmEmber();
        p.Background = QColor(0x14, 0x10, 0x1E);
        p.BgAlt = QColor(0x10, 0x0C, 0x18);
        p.BgMid = QColor(0x12, 0x0E, 0x1A);
        p.Surface = QColor(0x1E, 0x18, 0x2C);
        p.SurfaceAlt = QColor(0x1A, 0x14, 0x28);
        p.Border = QColor(0x28, 0x20, 0x3C);
        p.BorderHover = QColor(0x38, 0x2C, 0x50);
        p.Accent = QColor(0xC0, 0xA8, 0xF0);
        p.AccentHover = QColor(0xD4, 0xC0, 0xFF);
        p.AccentDim = QColor(0x78, 0x60, 0xB0);
        p.AccentBg = QColor(0x22, 0x14, 0x38);
        p.Text = QColor(0xD0, 0xC8, 0xE8);
        p.TextMuted = QColor(0x80, 0x70, 0xA8);
        p.NodeBorderSelected = QColor(0xC0, 0xA8, 0xF0);
        p.WirePending = QColor(0xD0, 0x80, 0xFF);
        p.ValueBoxActive = QColor(0xC0, 0xA8, 0xF0);
        return p;
    }

    ThemePalette ThemeManager::MakeMidnightRose()
    {
        ThemePalette p = MakeWarmEmber();
        p.Background = QColor(0x1A, 0x10, 0x14);
        p.BgAlt = QColor(0x14, 0x0C, 0x10);
        p.BgMid = QColor(0x16, 0x0E, 0x12);
        p.Surface = QColor(0x24, 0x16, 0x1C);
        p.SurfaceAlt = QColor(0x20, 0x12, 0x18);
        p.Border = QColor(0x30, 0x1E, 0x26);
        p.BorderHover = QColor(0x40, 0x2A, 0x34);
        p.Accent = QColor(0xF0, 0x68, 0x88);
        p.AccentHover = QColor(0xFF, 0x80, 0xA0);
        p.AccentDim = QColor(0xA0, 0x38, 0x58);
        p.AccentBg = QColor(0x2C, 0x10, 0x18);
        p.Text = QColor(0xE8, 0xC8, 0xD4);
        p.TextMuted = QColor(0x98, 0x68, 0x78);
        p.NodeBorderSelected = QColor(0xF0, 0x68, 0x88);
        p.WirePending = QColor(0xFF, 0x60, 0xA0);
        p.ValueBoxActive = QColor(0xF0, 0x68, 0x88);
        return p;
    }
}