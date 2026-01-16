cmake_minimum_required(VERSION 3.15)

project(NuTests)

enable_testing()

add_subdirectory(NuUnitTests)
add_subdirectory(NuBenchmarks)

set_property(TARGET NuUnitTests PROPERTY FOLDER "Tests")
set_property(TARGET NuBenchmarks PROPERTY FOLDER "Tests")