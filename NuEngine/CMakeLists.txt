cmake_minimum_required(VERSION 3.16)
project(NuEngine)

file(GLOB_RECURSE ENGINE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glad/src/*.c" 
)

add_library(NuEngine STATIC ${ENGINE_SRC})

target_compile_features(NuEngine PUBLIC cxx_std_20)

target_include_directories(NuEngine PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"

    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"

    "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glad/include"
)

find_package(OpenGL REQUIRED)

target_link_libraries(NuEngine
    PUBLIC
        NuMath
        Glad
        glm::glm
        OpenGL::GL
)

if (WIN32)
    target_compile_definitions(NuEngine PRIVATE "_CRT_SECURE_NO_WARNINGS" "NOMINMAX")
endif()