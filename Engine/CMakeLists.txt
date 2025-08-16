file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS *.cpp *.hpp)

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/include
)

add_subdirectory(Math)

target_link_libraries(Engine PUBLIC Math glfw glad)

find_package(OpenGL REQUIRED)
target_link_libraries(Engine PUBLIC OpenGL::GL)

if (WIN32)
    target_compile_definitions(Engine PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()





