cmake_minimum_required(VERSION 3.10.0)
project(lighting VERSION 0.1.0 LANGUAGES C CXX)
include_directories(SYSTEM "include")

add_executable(lighting src/main.cpp src/glad.c)

target_link_libraries(lighting glfw)

