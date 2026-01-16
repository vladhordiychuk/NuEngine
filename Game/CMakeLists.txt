project(Game)

add_executable(Game 
    main.cpp
)

target_compile_features(Game PRIVATE cxx_std_20)

target_link_libraries(Game PRIVATE NuEngine)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources"
    "$<TARGET_FILE_DIR:Game>/Resources"
)