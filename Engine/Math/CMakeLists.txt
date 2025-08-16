cmake_minimum_required(VERSION 3.16)
project(Math)

option(NU_USE_SSE "Use SSE backend for SIMD" ON)
option(NU_USE_NEON "Use NEON backend for SIMD" OFF)

# Збираємо всі заголовки (не обов'язково, але зручно для IDE)
file(GLOB_RECURSE MATH_HEADERS CONFIGURE_DEPENDS "include/Math/*.hpp")

# Header-only бібліотека
add_library(Math INTERFACE)

# Папка з include
target_include_directories(Math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Стандарт C++
target_compile_features(Math INTERFACE cxx_std_20)

# SIMD опції
if(NU_USE_SSE)
    target_compile_definitions(Math INTERFACE NU_USE_SSE)
elseif(NU_USE_NEON)
    target_compile_definitions(Math INTERFACE NU_USE_NEON)
else()
    message(STATUS "Using scalar fallback for SIMD")
endif()

# Додаткові флаги для Clang/GCC
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    if(NU_USE_SSE)
        target_compile_options(Math INTERFACE -msse4.1)
    endif()
endif()




