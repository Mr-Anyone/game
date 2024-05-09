set(SOURCE_FILES
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    misc/cpp/imgui_stdlib.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_glfw.cpp
)

add_library(imgui
    ${SOURCE_FILES}
)
target_link_libraries(imgui glfw)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/backends ${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp)
