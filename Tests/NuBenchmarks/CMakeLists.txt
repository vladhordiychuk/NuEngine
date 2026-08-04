project(NuBenchmarks)

file(GLOB_RECURSE BENCH_SOURCES "src/*.cpp")

add_executable(NuBenchmarks 
    ${BENCH_SOURCES}
    main.cpp
)

set_target_properties(NuBenchmarks PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(NuBenchmarks PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(NuBenchmarks PRIVATE 
    NuMath
    NuEngine
    benchmark::benchmark
    glm::glm
)
