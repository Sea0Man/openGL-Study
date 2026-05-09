cmake_minimum_required(VERSION 3.10.0)
project(lit_up_model VERSION 0.1.0 LANGUAGES C CXX)
include_directories(SYSTEM "include")

add_executable(lit_up_model src/main.cpp src/glad.c)

target_link_libraries(lit_up_model glfw -lassimp)

