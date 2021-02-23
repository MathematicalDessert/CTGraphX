# CS-255 Graphics Assignment

cmake_minimum_required(VERSION 3.0)

# Project
project(ctgraphx CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)
find_package(spdlog REQUIRED)

# IMGUI project
add_subdirectory(imgui)

# Include dirs
include_directories(include)

# Create executable
add_executable(
	ctgraphx
	src/main.cpp
	src/application.cpp
	src/window/window.cpp
	src/window/render_settings.cpp
	src/window/debug_panel.cpp
	src/ct/ct_head.cpp
	src/window/ct_head_viewer.cpp)

# Link necessary libraries
target_link_libraries(ctgraphx imgui ${OpenGL_LIBRARIES} ${SDL2_LIBRARIES} Opengl32 OpenCL::OpenCL spdlog::spdlog spdlog::spdlog_header_only)

# Install application
install(TARGETS ctgraphx RUNTIME DESTINATION bin)