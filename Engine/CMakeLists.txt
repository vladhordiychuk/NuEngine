file(GLOB_RECURSE ENGINE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_subdirectory(Math)

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/include
) 

target_link_libraries(Engine
    PUBLIC Math
)

find_package(OpenGL REQUIRED)

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/thirdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/glad/include)

target_link_libraries(Engine PUBLIC glfw glad OpenGL::GL)

if (WIN32)
    target_compile_definitions(Engine PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()




