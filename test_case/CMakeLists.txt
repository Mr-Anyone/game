# test entity component system 
add_executable(test_ecs test_ecs.cpp)
target_link_libraries(test_ecs glm ecs)
target_include_directories(test_ecs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# test collision
add_executable(test_collision_solver collision_solver_test.cpp)
target_link_libraries(test_collision_solver physics ecs)
target_include_directories(test_collision_solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
