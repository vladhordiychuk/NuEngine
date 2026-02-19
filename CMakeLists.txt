cmake_minimum_required(VERSION 3.16)
project(NuEngineWorkspace) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(NU_BUILD_TESTS "Build unit tests" ON)

if (MSVC)
    add_compile_options(/W4 /permissive- /MP)
    add_compile_definitions(UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(ThirdParty) 

add_subdirectory(NuEngine) 

add_subdirectory(NuEngine/NuMath)

add_subdirectory(Game)

add_subdirectory(NuEditor)

if (NU_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()