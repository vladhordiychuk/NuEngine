project(Math)

option(NU_USE_SSE "Use SSE backend for SIMD" ON)
option(NU_USE_NEON "Use NEON backend for SIMD" OFF)

file(GLOB_RECURSE MATH_HEADERS CONFIGURE_DEPENDS "include/Math/*.hpp")

add_library(Math INTERFACE)
target_include_directories(Math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(Math INTERFACE cxx_std_20)

if(NU_USE_SSE)
    target_compile_definitions(Math INTERFACE NU_USE_SSE)
elseif(NU_USE_NEON)
    target_compile_definitions(Math INTERFACE NU_USE_NEON)
else()
    message(STATUS "Using scalar fallback for SIMD")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    if(NU_USE_SSE)
        target_compile_options(Math INTERFACE -msse4.1)
    endif()
endif()
