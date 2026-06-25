#pragma once
#include <QString>
#include <QSettings>

namespace NuEditor
{
    enum class EditorTheme
    {
        WarmEmber,
        VolcanicAsh,
        ArcticSlate,
        ForestDepth,
        DuskPurple,
        MidnightRose
    };

    class EditorPreferences
    {
    public:
        static EditorPreferences& Get()
        {
            static EditorPreferences instance;
            return instance;
        }

        [[nodiscard]] EditorTheme GetTheme() const { return m_Theme; }
        void SetTheme(EditorTheme theme) { m_Theme = theme; }

        void Load()
        {
            QSettings s("NuEditor", "Preferences");
            m_Theme = static_cast<EditorTheme>(s.value("appearance/theme", 0).toInt());
        }

        void Save()
        {
            QSettings s("NuEditor", "Preferences");
            s.setValue("appearance/theme", static_cast<int>(m_Theme));
        }

    private:
        EditorPreferences() = default;
        EditorTheme m_Theme = EditorTheme::WarmEmber;
    };
}