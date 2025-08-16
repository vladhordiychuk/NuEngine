file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Math/include
)

target_link_libraries(Engine PUBLIC
    Math
    glfw
    glad
)

if (WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Engine PUBLIC ${OPENGL_LIBRARIES})
    target_include_directories(Engine PUBLIC ${OPENGL_INCLUDE_DIR})
    target_compile_definitions(Engine PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()