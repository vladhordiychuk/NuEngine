file(GLOB_RECURSE ENGINE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    # НЕ додавай *.hpp сюди, вони не потрібні в sources
)

add_subdirectory(NuMath)

add_library(Engine STATIC ${ENGINE_SRC})

# Додаємо шляхи для інклудів Engine, якщо потрібно
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Підключаємо NuMath як бібліотеку, щоб Engine міг користуватися математикою
target_link_libraries(Engine PUBLIC NuMath)

# Далі твої залежності
find_package(OpenGL REQUIRED)

add_subdirectory(../thirdparty/glfw glfw_build)
add_library(glad STATIC ../thirdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC ../thirdparty/glad/include)

target_link_libraries(Engine PUBLIC glfw glad OpenGL::GL)

if (WIN32)
    target_compile_definitions(Engine PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()

