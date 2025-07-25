file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_subdirectory(Math)

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Engine PUBLIC NuMath)

find_package(OpenGL REQUIRED)

add_subdirectory(../thirdparty/glfw glfw_build)
add_library(glad STATIC ../thirdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC ../thirdparty/glad/include)

target_link_libraries(Engine PUBLIC glfw glad OpenGL::GL)

if (WIN32)
    target_compile_definitions(Engine PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()
