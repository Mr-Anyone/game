add_library(libglad SHARED src/glad.c)

message(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(libglad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
