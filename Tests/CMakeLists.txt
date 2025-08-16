project(Tests)

file(GLOB_RECURSE UNIT_TEST_SOURCES CONFIGURE_DEPENDS Unit/*.cpp)
file(GLOB_RECURSE BENCHMARK_SOURCES CONFIGURE_DEPENDS Benchmark/*.cpp)

add_executable(Tests
    test_main.cpp
    ${UNIT_TEST_SOURCES}
    ${BENCHMARK_SOURCES}
)

target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(Tests PRIVATE
    Engine
    gtest_main
    benchmark::benchmark
    glm::glm
)

include(GoogleTest)
gtest_discover_tests(Tests)


