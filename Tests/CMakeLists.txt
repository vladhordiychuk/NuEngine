file(GLOB_RECURSE UNIT_TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Unit/*.cpp)

add_executable(Tests
    testMain.cpp
    ${UNIT_TEST_SOURCES}
)

target_link_libraries(Tests PRIVATE Engine gtest_main glm::glm)
target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR})

enable_testing()
include(GoogleTest)
gtest_discover_tests(Tests PROPERTIES TIMEOUT 60)

option(RUN_BENCHMARKS "Build benchmark tests" OFF)

if(RUN_BENCHMARKS)
    file(GLOB_RECURSE BENCHMARK_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Benchmark/*.cpp)

    add_executable(Benchmarks
        testMain.cpp
        ${BENCHMARK_SOURCES}
    )
    target_compile_definitions(Benchmarks PRIVATE RUN_BENCHMARKS)
    target_link_libraries(Benchmarks PRIVATE Engine benchmark::benchmark glm::glm)
    target_include_directories(Benchmarks PRIVATE ${CMAKE_SOURCE_DIR})
endif()

