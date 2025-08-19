if(DEFINED ENV{CI})
    add_compile_definitions(CI)
endif()

file(GLOB_RECURSE UNIT_TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Unit/*.cpp
)

file(GLOB_RECURSE BENCHMARK_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Benchmark/*.cpp
)

add_executable(Tests
    testMain.cpp
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
include(GoogleTest)
gtest_discover_tests(Tests)
