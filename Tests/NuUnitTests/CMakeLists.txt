project(NuUnitTests)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(NuUnitTests ${TEST_SOURCES})

target_link_libraries(NuUnitTests PRIVATE 
    NuMath 
    GTest::gtest 
    GTest::gtest_main
)