#pragma once
#include <QObject>
#include <QColor>
#include <QString>
#include <Core/EditorPreferences.hpp>

namespace NuEditor::Core
{
    struct ThemePalette
    {
        QColor Background;
        QColor BgAlt;
        QColor BgMid;
        QColor Surface;
        QColor SurfaceAlt;

        QColor Border;
        QColor BorderHover;

        QColor Accent;
        QColor AccentHover;
        QColor AccentDim;
        QColor AccentBg;

        QColor Text;
        QColor TextMuted;
        QColor TextOff;

        QColor GridLine;
        QColor GridDot;
        QColor MinimapBg;
        QColor MinimapBorder;
        QColor MinimapView;

        QColor NodeBg;
        QColor NodeBorder;
        QColor NodeBorderSelected;
        QColor PinBg;
        QColor WirePending;
        QColor ValueBoxBg;
        QColor ValueBoxBorder;
        QColor ValueBoxActive;

        QColor Play;
        QColor Pause;
        QColor Stop;
    };

    class ThemeManager : public QObject
    {
        Q_OBJECT
    public:
        [[nodiscard]] static ThemeManager& Get()
        {
            static ThemeManager instance;
            return instance;
        }

        void Apply(EditorTheme theme);

        void ApplyCurrent();

        [[nodiscard]] const ThemePalette& Palette() const { return m_Palette; }
        [[nodiscard]] EditorTheme         Current()  const { return m_Current; }

        [[nodiscard]] static QString QssPath(EditorTheme theme);

    signals:
        void ThemeChanged();

    private:
        ThemeManager();

        void ApplyQss(EditorTheme theme);
        void BuildPalette(EditorTheme theme);

        static ThemePalette MakeWarmEmber();
        static ThemePalette MakeVolcanicAsh();
        static ThemePalette MakeArcticSlate();
        static ThemePalette MakeForestDepth();
        static ThemePalette MakeDuskPurple();
        static ThemePalette MakeMidnightRose();

        ThemePalette m_Palette;
        EditorTheme  m_Current = EditorTheme::WarmEmber;
    };
}