cmake_minimum_required(VERSION 4.2)
project(Chip8Interpreter C CXX)

option(MINIFB_USE_METAL_API "Use Metal API" ON)

set(CMAKE_C_STANDARD 11)

add_subdirectory(dependencies/minifb)

add_executable(Chip8Interpreter main.c
        hardware.cpp
        hardware.h)

target_link_libraries(${PROJECT_NAME} minifb)