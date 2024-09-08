cmake_minimum_required(VERSION 3.10.0)
enable_testing()

project(GameEngineAndGame)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor)
add_subdirectory(src)
