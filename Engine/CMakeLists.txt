file(GLOB_RECURSE ENGINE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/glad/src/*.c
)

add_subdirectory(Math)

add_library(Engine STATIC ${ENGINE_SRC})

# Include-шляхи
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/thirdparty/glad/include    
)

# Лінкування
find_package(OpenGL REQUIRED)

target_link_libraries(Engine
    PUBLIC
        Math
        OpenGL::GL
)

# Визначення для Windows
if (WIN32)
    target_compile_definitions(Engine PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()