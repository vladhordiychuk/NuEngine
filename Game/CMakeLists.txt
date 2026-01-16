project(Game)

add_executable(Game 
    main.cpp
)

target_compile_features(Game PRIVATE cxx_std_20)

target_link_libraries(Game PRIVATE NuEngine)