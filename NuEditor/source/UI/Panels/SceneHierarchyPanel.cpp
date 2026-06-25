#include <UI/Panels/SceneHierarchyPanel.hpp>

#include <QVBoxLayout>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

namespace NuEditor
{
    SceneHierarchyPanel::SceneHierarchyPanel(QWidget* parent)
        : QDockWidget("SCENE", parent)
    {
        setObjectName("sceneHierarchyPanel");
        setFeatures(QDockWidget::DockWidgetMovable |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetClosable);

        SetupUI();
        SetupConnections();
        PopulateDemoScene();
    }

    void SceneHierarchyPanel::SetupUI()
    {
        QWidget* content = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(content);
        layout->setContentsMargins(4, 4, 4, 4);
        layout->setSpacing(4);

        m_SearchBar = new SearchLineEdit(content);
        m_SearchBar->setObjectName("searchBar");
        m_SearchBar->setCustomPlaceholder("Search...");
        layout->addWidget(m_SearchBar);

        m_SceneTree = new QTreeWidget(content);
        m_SceneTree->setObjectName("sceneTree");
        m_SceneTree->setHeaderHidden(true);
        m_SceneTree->setColumnCount(1);
        m_SceneTree->setIndentation(16);
        m_SceneTree->setAnimated(true);
        m_SceneTree->setExpandsOnDoubleClick(true);
        m_SceneTree->setSelectionMode(QAbstractItemView::SingleSelection);
        m_SceneTree->setContextMenuPolicy(Qt::CustomContextMenu);
        m_SceneTree->setAttribute(Qt::WA_MacShowFocusRect, false);

        m_SceneTree->setRootIsDecorated(true);

        m_SceneTree->setStyle(QStyleFactory::create("Fusion"));

        layout->addWidget(m_SceneTree);
        setWidget(content);
    }

    void SceneHierarchyPanel::SetupConnections()
    {
        connect(m_SearchBar, &SearchLineEdit::textChanged,
            this, &SceneHierarchyPanel::OnSearchTextChanged);

        connect(m_SceneTree, &QTreeWidget::itemSelectionChanged,
            this, &SceneHierarchyPanel::OnItemSelectionChanged);

        connect(m_SceneTree, &QTreeWidget::itemDoubleClicked,
            this, &SceneHierarchyPanel::OnItemDoubleClicked);

        connect(m_SceneTree, &QTreeWidget::customContextMenuRequested,
            this, &SceneHierarchyPanel::OnCustomContextMenu);
    }

    void SceneHierarchyPanel::PopulateDemoScene()
    {
        m_SceneTree->clear();

        QTreeWidgetItem* sceneRoot = new QTreeWidgetItem(m_SceneTree);
        sceneRoot->setText(0, "Scene");
        sceneRoot->setData(0, ItemTypeRole, static_cast<int>(SceneItemType::Folder));
        sceneRoot->setData(0, EntityIdRole, 0);
        sceneRoot->setIcon(0, QIcon(":/icons/folder.svg"));
        sceneRoot->setExpanded(true);

        MakeItem(sceneRoot, "Cube_001", SceneItemType::Entity, 1);
        MakeItem(sceneRoot, "Cube_002", SceneItemType::Entity, 2);
        MakeItem(sceneRoot, "Cube_003", SceneItemType::Entity, 3);

        QTreeWidgetItem* propsFolder = MakeItem(sceneRoot, "Props", SceneItemType::Folder, 10);
        propsFolder->setExpanded(true);
        MakeItem(propsFolder, "Cube_004", SceneItemType::Entity, 4);
        MakeItem(propsFolder, "Cube_005", SceneItemType::Entity, 5);

        MakeItem(sceneRoot, "DirectionalLight", SceneItemType::Light, 20);
        MakeItem(sceneRoot, "Camera_Main", SceneItemType::Camera, 30);

        m_SceneTree->setCurrentItem(sceneRoot->child(0));
    }

    QTreeWidgetItem* SceneHierarchyPanel::MakeItem(
        QTreeWidgetItem* parent,
        const QString& label,
        SceneItemType    type,
        int              entityId,
        const QIcon& icon)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setText(0, label);
        item->setData(0, ItemTypeRole, static_cast<int>(type));
        item->setData(0, EntityIdRole, entityId);
        item->setData(0, VisibleRole, true);

        if (!icon.isNull())
        {
            item->setIcon(0, icon);
        }
        else
        {
            switch (type)
            {
            case SceneItemType::Entity: item->setIcon(0, QIcon(":/icons/cube.svg"));    break;
            case SceneItemType::Folder: item->setIcon(0, QIcon(":/icons/folder.svg"));  break;
            case SceneItemType::Light:  item->setIcon(0, QIcon(":/icons/light.svg"));   break;
            case SceneItemType::Camera: item->setIcon(0, QIcon(":/icons/camera.svg"));  break;
            }
        }

        return item;
    }

    void SceneHierarchyPanel::OnSearchTextChanged(const QString& text)
    {
        FilterTree(text);
    }

    void SceneHierarchyPanel::FilterTree(const QString& text)
    {
        QTreeWidgetItem* root = m_SceneTree->invisibleRootItem();
        for (int i = 0; i < root->childCount(); ++i)
            FilterItem(root->child(i), text);
    }

    void SceneHierarchyPanel::FilterItem(QTreeWidgetItem* item, const QString& text)
    {
        if (!item) return;

        bool childVisible = false;
        for (int i = 0; i < item->childCount(); ++i)
        {
            FilterItem(item->child(i), text);
            if (!item->child(i)->isHidden())
                childVisible = true;
        }

        if (text.isEmpty())
        {
            item->setHidden(false);
        }
        else
        {
            const bool match = item->text(0).contains(text, Qt::CaseInsensitive);
            item->setHidden(!match && !childVisible);
            if (childVisible)
                item->setExpanded(true);
        }
    }

    void SceneHierarchyPanel::OnItemSelectionChanged()
    {
        const auto items = m_SceneTree->selectedItems();
        if (items.isEmpty()) return;
        emit EntitySelected(items.first()->data(0, EntityIdRole).toInt());
    }

    int SceneHierarchyPanel::SelectedEntityId() const
    {
        const auto items = m_SceneTree->selectedItems();
        if (items.isEmpty()) return -1;
        return items.first()->data(0, EntityIdRole).toInt();
    }

    void SceneHierarchyPanel::OnItemDoubleClicked(QTreeWidgetItem* item, int)
    {
        if (item) m_SceneTree->editItem(item, 0);
    }

    void SceneHierarchyPanel::OnCustomContextMenu(const QPoint& pos)
    {
        QTreeWidgetItem* item = m_SceneTree->itemAt(pos);
        QMenu menu(this);

        if (item)
        {
            const auto type = static_cast<SceneItemType>(
                item->data(0, ItemTypeRole).toInt());

            QAction* renameAct = menu.addAction("Rename");
            connect(renameAct, &QAction::triggered, [this, item]() {
                m_SceneTree->editItem(item, 0);
                });

            if (type == SceneItemType::Entity)
            {
                menu.addSeparator();

                QAction* dupAct = menu.addAction("Duplicate");
                connect(dupAct, &QAction::triggered, [this, item]() {
                    QTreeWidgetItem* parent = item->parent()
                        ? item->parent()
                        : m_SceneTree->invisibleRootItem();
                    auto* copy = item->clone();
                    copy->setText(0, item->text(0) + "_Copy");
                    parent->addChild(copy);
                    });

                menu.addSeparator();

                QAction* deleteAct = menu.addAction("Delete");
                connect(deleteAct, &QAction::triggered, [item]() {
                    delete item;
                    });
            }

            menu.addSeparator();

            const bool visible = item->data(0, VisibleRole).toBool();
            QAction* visAct = menu.addAction(visible ? "Hide" : "Show");
            connect(visAct, &QAction::triggered, [this, item, visible]() {
                item->setData(0, VisibleRole, !visible);

                if (!visible)
                {
                    item->setForeground(0, QBrush());
                }
                else
                {
                    item->setForeground(0, QColor("#3A3428"));
                }

                emit EntityVisibilityToggled(
                    item->data(0, EntityIdRole).toInt(), !visible);
                });
        }
        else
        {
            QAction* newFolderAct = menu.addAction("New Folder");
            connect(newFolderAct, &QAction::triggered, [this]() {
                QTreeWidgetItem* root = m_SceneTree->invisibleRootItem();
                auto* folder = MakeItem(root->child(0), "New Folder", SceneItemType::Folder, -1);
                folder->setExpanded(true);
                m_SceneTree->editItem(folder, 0);
                });
        }

        menu.exec(m_SceneTree->viewport()->mapToGlobal(pos));
    }
} // namespace NuEditor