cmake_minimum_required(VERSION 3.10.0)

enable_testing()

project(DiamondSquare)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(vendor)


add_executable(main main.cpp ds.cpp ecs.cpp physics.cpp solve.cpp)
target_link_libraries(main glfw glm libglad imgui)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(test_ecs test/test_ecs.cpp ecs.cpp)
target_link_libraries(test_ecs glm)
target_include_directories(test_ecs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(test_collision_solver test/collision_solver_test.cpp physics.cpp ecs.cpp solve.cpp)
target_link_libraries(test_collision_solver glm libglad glfw)
target_include_directories(test_collision_solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_test(
    NAME test_ecs
    COMMAND $<TARGET_FILE:test_ecs>
)

add_test(
    NAME test_collision
    COMMAND $<TARGET_FILE:test_collision_solver>)

