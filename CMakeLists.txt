cmake_minimum_required(VERSION 3.16)
project(NuEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter)
endif()

add_subdirectory(thirdparty/glm ${CMAKE_BINARY_DIR}/glm)
add_subdirectory(thirdparty/googletest ${CMAKE_BINARY_DIR}/googletest)
add_subdirectory(thirdparty/benchmark ${CMAKE_BINARY_DIR}/benchmark)
add_subdirectory(thirdparty/glfw ${CMAKE_BINARY_DIR}/glfw)

add_subdirectory(Engine)
add_subdirectory(Game)

enable_testing()
add_subdirectory(Tests)