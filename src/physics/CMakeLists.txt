add_subdirectory(ecs)

add_library(
    physics
    physics.cpp
    solve.cpp
)

target_link_libraries(physics PRIVATE glm glad ecs)
#not a good idea to expose implementation but it works!
target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
