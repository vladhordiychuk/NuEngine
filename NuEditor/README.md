# NuEditor

**NuEditor** is the official visual editor developed as a fully integrated module of the **NuEngine** engine. Its purpose is to provide a seamless environment for scene manipulation, visual programming (via the Weave VM), and real-time rendering, while strictly maintaining the core engine's philosophy and data-oriented approach.

---

## 📸 Gallery (Screenshots)

### Main Window and Viewport
![NuEditor Main Window](path_to_main_image.png)
*The editor interface featuring an active OpenGL Viewport, scene hierarchy, and object inspector.*

### Weave: Visual Programming
![Weave Graph Editor](path_to_weave_image.png)
*The Weave node-based logic editor, which interacts directly with NuEngine's virtual machine.*

### Custom UI Themes
![Theme Settings](path_to_themes_image.png)
*Demonstration of the custom UI and its theme system (Arctic Slate, Warm Ember, etc.).*

---

## ✨ Core Module Features

- **Deep Integration with NuEngine:** Full access to the engine's entities, components, and resources without the need for complex "bridges" or intermediate layers.
- **Interactive Viewport (`NuOpenGLWidget`):** A custom OpenGL rendering window supporting real-time scene manipulation (Translate, Rotate, Scale).
- **Weave Editor (`WeaveGraphScene` & `WeaveCompiler`):** A visual graph editor, compiler, and serializer for rapidly building game logic that compiles directly into bytecode.
- **Custom UI Toolkit:** A comprehensive set of Qt-based dockable panels (Scene Hierarchy, Inspector, Content Drawer, Console) featuring custom frameless window styling (`CustomTitleBar`).
- **Runtime Management (`EditorRuntime`, `GameLoader`):** Control the game's lifecycle (Play, Pause, Stop) and test the simulation natively within the editor.

---

## 📂 Module Structure

All editor logic is encapsulated and cleanly separated into functional blocks:

- `source/App/` — Module entry point, main window initialization (`EditorWindow`, `ResizableWindow`).
- `source/Core/` — Editor state management, preferences, and styling (`ThemeManager`).
- `source/Runtime/` — Communication with the NuEngine core, game DLL loading, and game loop control.
- `source/UI/` — All interface elements (panels, widgets, toolbars, menus).
- `source/Viewport/` — Graphics API (OpenGL) integration within the editor window.
- `source/Weave/` — Weave graph editor logic, tooling, parsing, and compilation.
- `resources/` — Module assets: SVG icons and QSS stylesheets.

---

## 🚀 Integration & Build

Because **NuEditor** is an integrated module, it is not built in isolation but rather as part of the overarching **NuEngine** CMake tree. 

To build the module successfully, ensure your environment supports **C++20** and has **Qt 5/6** installed.

1. In the root `CMakeLists.txt` of NuEngine, the module should be included like this:

   ```cmake
   add_subdirectory(NuEditor)
   ```
2. Build the project normally from the engine's root directory:

   ```cmake
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release
   ```
