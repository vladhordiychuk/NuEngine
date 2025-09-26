file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_executable(Game ${GAME_SRC})

target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/Math/include
)

target_link_libraries(Game PRIVATE Engine)