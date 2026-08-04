add_subdirectory(glm)

# ==========================
# Glad
# ==========================
add_library(Glad STATIC
    glad/src/glad.c
    glad/include/glad/glad.h
)

target_include_directories(Glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# ==========================
# stb
# ==========================
add_library(stb STATIC
    stb/stb_image.cpp
)

target_include_directories(stb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ==========================
# EnTT
# ==========================
add_library(entt INTERFACE)

target_include_directories(entt INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/entt
)

# ==========================
# Jolt Physics
# ==========================
set(TARGET_UNIT_TESTS OFF CACHE BOOL "Turn off Jolt tests" FORCE)
set(TARGET_HELLO_WORLD OFF CACHE BOOL "Turn off Jolt examples" FORCE)
set(TARGET_PERFORMANCE_TEST OFF CACHE BOOL "Turn off Jolt perf tests" FORCE)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "Use dynamic runtime for Jolt" FORCE)

add_subdirectory(JoltPhysics/Build)

# ==========================
# Tests
# ==========================
if (NU_BUILD_TESTS)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory(googletest)

    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    add_subdirectory(benchmark)
endif()