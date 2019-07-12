cmake_minimum_required(VERSION 3.0)

project(triangle C)

set(TARGET_NAME triangle_console)

add_executable(${TARGET_NAME} triangle.c)
target_link_libraries(${TARGET_NAME} triangle)
