project(DiamondSquare)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(physics)

# the diamond square file
add_library(
    diamond_square
    ds.cpp
)

target_link_libraries(diamond_square 
    glad 
    glm
)
target_include_directories(diamond_square 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# the sandbox files
add_executable(sandbox 
    main.cpp 
    # ds.cpp 
    # ecs.cpp
)

target_link_libraries(sandbox 
    # third party
    glfw 
    glm 
    glad 
    imgui 

    # my libraries
    diamond_square 
    ecs
    physics
)
target_include_directories(sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
