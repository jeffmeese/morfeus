cmake_minimum_required(VERSION 3.0)

project(triangle C)

set(TRIANGLE_NAME triangle)
set(LIBTRI_NAME libtri)

add_executable(${TRIANGLE_NAME} triangle.h triangle.c)
add_library(${LIBTRI_NAME} triangle.h triangle.c)

target_compile_options(${LIBTRI_NAME} PUBLIC -DTRILIBRARY)
if (MSVC)
  target_compile_definitions(${LIBTRI_NAME} PUBLIC -DNO_TIMER)
  target_compile_definitions(${LIBTRI_NAME} PUBLIC -D_CRT_SECURE_NO_WARNINGS)
  target_compile_options(${LIBTRI_NAME} PUBLIC /wd4311 /wd4312 /wd4244 /wd4477)
  
  target_compile_definitions(${TRIANGLE_NAME} PUBLIC -DNO_TIMER)
  target_compile_definitions(${TRIANGLE_NAME} PUBLIC -D_CRT_SECURE_NO_WARNINGS)
  target_compile_options(${TRIANGLE_NAME} PUBLIC /wd4311 /wd4312 /wd4244 /wd4477)
endif()

