file(GLOB_RECURSE UNIT_TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Unit/*.cpp
)

file(GLOB_RECURSE BENCHMARK_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Benchmark/*.cpp
)

add_executable(Tests
    main.cpp
    ${UNIT_TEST_SOURCES}
    ${BENCHMARK_SOURCES}
)

target_include_directories(Tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(Tests PRIVATE
    Engine
    gtest_main
    benchmark::benchmark
    glm::glm
)

enable_testing()

set(GTEST_DISCOVERY_TIMEOUT 60) 
set(GTEST_TIMEOUT 300)

option(RUN_BENCHMARKS "Run benchmark tests" OFF)

include(GoogleTest)
if(RUN_BENCHMARKS)
    gtest_discover_tests(Tests
        DISCOVERY_TIMEOUT 60
        PROPERTIES TIMEOUT 120
    )
else()
    gtest_discover_tests(Tests
        DISCOVERY_TIMEOUT 30
        PROPERTIES TIMEOUT 60
        TEST_FILTER "-*Benchmark*"
    )
endif()
