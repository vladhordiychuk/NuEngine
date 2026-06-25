#pragma once
#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <UI/Widgets/SearchLineEdit.hpp>

namespace NuEditor
{
    enum SceneItemRole
    {
        ItemTypeRole = Qt::UserRole,
        EntityIdRole,
        VisibleRole
    };

    enum class SceneItemType
    {
        Entity,
        Folder,
        Light,
        Camera
    };

    class SceneHierarchyPanel : public QDockWidget
    {
        Q_OBJECT

    public:
        explicit SceneHierarchyPanel(QWidget* parent = nullptr);

        void PopulateDemoScene();
        int  SelectedEntityId() const;

    signals:
        void EntitySelected(int entityId);
        void EntityVisibilityToggled(int entityId, bool visible);

    private slots:
        void OnSearchTextChanged(const QString& text);
        void OnItemSelectionChanged();
        void OnItemDoubleClicked(QTreeWidgetItem* item, int column);
        void OnCustomContextMenu(const QPoint& pos);

    private:
        QTreeWidget* m_SceneTree = nullptr;
        SearchLineEdit* m_SearchBar = nullptr;

        void SetupUI();
        void SetupConnections();
        void FilterTree(const QString& text);
        void FilterItem(QTreeWidgetItem* item, const QString& text);

        QTreeWidgetItem* MakeItem(QTreeWidgetItem* parent,
            const QString& label,
            SceneItemType    type,
            int              entityId,
            const QIcon& icon = {});
    };

} // namespace NuEditor