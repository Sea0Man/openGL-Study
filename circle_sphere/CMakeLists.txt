cmake_minimum_required(VERSION 3.10.0)
project(circle_sphere VERSION 0.1.0 LANGUAGES C CXX)
include_directories(SYSTEM "include")

add_executable(circle_sphere src/main.cpp src/glad.c)

target_link_libraries(circle_sphere glfw)

