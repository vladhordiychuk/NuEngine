cmake_minimum_required(VERSION 3.16)

project(NuMath)

option(NU_USE_SSE "Use SSE backend for SIMD" ON)
option(NU_USE_NEON "Use NEON backend for SIMD" OFF)

file(GLOB_RECURSE MATH_HEADERS CONFIGURE_DEPENDS "include/NuMath/*.hpp")

set(NATVIS_FILE "NuMath.natvis")

add_library(NuMath INTERFACE)

target_sources(NuMath INTERFACE 
    ${MATH_HEADERS}
    ${NATVIS_FILE} 
)

source_group("Visualizers" FILES ${NATVIS_FILE})


target_include_directories(NuMath INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(NuMath INTERFACE cxx_std_20)

if(NU_USE_SSE)
    message(STATUS "[NuMath] Backend: SSE4.2")
    target_compile_definitions(NuMath INTERFACE NU_MATH_BACKEND=1)
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(NuMath INTERFACE -msse4.2)
    endif()

elseif(NU_USE_NEON)
    message(STATUS "[NuMath] Backend: NEON")
    target_compile_definitions(NuMath INTERFACE NU_MATH_BACKEND=2)
    
else()
    message(STATUS "[NuMath] Backend: Scalar (Fallback)")
    target_compile_definitions(NuMath INTERFACE NU_MATH_BACKEND=0)
endif()