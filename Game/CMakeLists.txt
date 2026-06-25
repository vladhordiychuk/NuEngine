project(Game)

add_library(Game SHARED
    source/SandboxApp.cpp
)
target_link_libraries(Game PRIVATE NuEngine)
target_compile_definitions(Game PRIVATE NU_EDITOR_MODE)

add_executable(GameStandalone
    source/SandboxApp.cpp
)
target_link_libraries(GameStandalone PRIVATE NuEngine)