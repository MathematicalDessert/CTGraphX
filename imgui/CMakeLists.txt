add_library(imgui
    # Main Imgui files
    imgui/imgui.cpp imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
    # SDL2+OpenGL-specific files
    imgui/backends/imgui_impl_sdl.cpp
    imgui/backends/imgui_impl_opengl2.cpp)

target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIR} imgui imgui/backends)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<SDL2/SDL_opengl.h>  GL_GLEXT_PROTOTYPES=1)

target_link_libraries(imgui PRIVATE SDL2::SDL2)