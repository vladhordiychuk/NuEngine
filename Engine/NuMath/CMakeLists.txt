project(NuMath LANGUAGES CXX)

file(GLOB_RECURSE NU_MATH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_library(NuMath STATIC ${NU_MATH_SOURCES})

target_include_directories(NuMath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(NuMath PUBLIC cxx_std_20)



