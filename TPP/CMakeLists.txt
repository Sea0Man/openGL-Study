cmake_minimum_required(VERSION 3.10.0)
project(TPP VERSION 0.1.0 LANGUAGES C CXX)
include_directories(SYSTEM "include")

add_executable(TPP src/main.cpp src/glad.c)

target_link_libraries(TPP glfw)

