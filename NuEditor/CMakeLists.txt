cmake_minimum_required(VERSION 3.16)
project(NuEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Svg)

set(PROJECT_SOURCES
    source/App/main.cpp
    source/App/ResizableWindow.cpp
    source/App/ResizableWindow.hpp
    source/App/EditorWindow.cpp
    source/App/EditorWindow.hpp

    source/Core/EditorPreferences.hpp
    source/Core/EditorState.hpp
    source/Core/ThemeManager.cpp
    source/Core/ThemeManager.hpp

    source/Runtime/EditorRuntime.cpp
    source/Runtime/EditorRuntime.hpp
    source/Runtime/GameLoader.cpp
    source/Runtime/GameLoader.hpp

    source/Viewport/EditorViewportHost.cpp
    source/Viewport/EditorViewportHost.hpp
    source/Viewport/NuOpenGLWidget.cpp
    source/Viewport/NuOpenGLWidget.hpp

    source/UI/EditorUI.cpp
    source/UI/EditorUI.hpp
    source/UI/Windows/SettingsWindow.cpp
    source/UI/Windows/SettingsWindow.hpp
    source/UI/Widgets/SearchLineEdit.cpp
    source/UI/Widgets/SearchLineEdit.hpp
    source/UI/Panels/ConsolePanel.cpp
    source/UI/Panels/ConsolePanel.hpp
    source/UI/Panels/ContentDrawerPanel.cpp
    source/UI/Panels/ContentDrawerPanel.hpp
    source/UI/Panels/CustomTitleBar.cpp
    source/UI/Panels/CustomTitleBar.hpp
    source/UI/Panels/InspectorPanel.cpp
    source/UI/Panels/InspectorPanel.hpp
    source/UI/Panels/MainMenuBar.cpp
    source/UI/Panels/MainMenuBar.hpp
    source/UI/Panels/MainToolBar.cpp
    source/UI/Panels/MainToolBar.hpp
    source/UI/Panels/SceneHierarchyPanel.cpp
    source/UI/Panels/SceneHierarchyPanel.hpp

    source/Weave/WeaveCompiler.cpp
    source/Weave/WeaveCompiler.hpp
    source/Weave/WeaveGraphScene.cpp
    source/Weave/WeaveGraphScene.hpp
    source/Weave/WeaveGraphView.cpp
    source/Weave/WeaveGraphView.hpp
    source/Weave/WeaveSerializer.cpp
    source/Weave/WeaveSerializer.hpp
    source/Weave/WeaveToolBar.cpp
    source/Weave/WeaveToolBar.hpp
    source/Weave/WeaveWindow.cpp
    source/Weave/WeaveWindow.hpp
    source/Weave/Utils/WeaveCommands.hpp

    resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NuEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(NuEditor
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(NuEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/source/App
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/source/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Viewport
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Weave
    ${CMAKE_SOURCE_DIR}/NuEngine
)

target_compile_definitions(NuEditor PRIVATE NU_EDITOR_MODE)

target_link_libraries(NuEditor PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::Svg
    NuEngine
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NuEditor)
endif()

set_target_properties(NuEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS NuEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NuEditor)
endif()