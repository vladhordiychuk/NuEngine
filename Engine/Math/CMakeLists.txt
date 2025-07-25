file(GLOB_RECURSE NU_MATH_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.cpp
)

add_library(NuMath STATIC ${NU_MATH_SOURCES})

target_include_directories(NuMath PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/include 
)



