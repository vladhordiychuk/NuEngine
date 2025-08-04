file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp
)

add_executable(UnitTests ${TEST_SOURCES})

target_link_libraries(UnitTests PRIVATE Engine gtest_main)

include(GoogleTest)
gtest_discover_tests(UnitTests)
