add_subdirectory(glm) 

add_library(Glad STATIC 
    glad/src/glad.c 
    glad/include/glad/glad.h
)

target_include_directories(Glad PUBLIC glad/include)

add_library(stb INTERFACE)

target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if (NU_BUILD_TESTS)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) 
    
    add_subdirectory(googletest)

    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    add_subdirectory(benchmark)
endif()