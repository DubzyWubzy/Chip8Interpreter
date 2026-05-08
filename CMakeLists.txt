cmake_minimum_required(VERSION 4.1)
project(Chip8Interpreter C CXX)

option(MINIFB_USE_METAL_API "Use Metal API" ON)

set(CMAKE_C_STANDARD 11)

add_subdirectory(dependencies/minifb)


add_library(display src/display.c)
target_include_directories(display PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(display PUBLIC minifb)

add_library(memory src/memory.c)
target_include_directories(memory PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(processor src/processor.c)
target_include_directories(processor PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(instructions src/instructions.c)
target_include_directories(instructions PUBLIC ${PROJECT_SOURCE_DIR}/include)

# TODO: add instruction libraries

add_executable(Chip8Interpreter src/main.c)
target_include_directories(Chip8Interpreter PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Chip8Interpreter PRIVATE display memory processor)
