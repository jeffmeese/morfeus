cmake_minimum_required(VERSION 3.0)

project(triangle C)

set(TARGET_NAME trilib)

add_library(${TARGET_NAME} STATIC trimesh.h trimesh.c pool.h pool.c mem.h mem.c reduced.h reduced.c cdt.c cdt.h types.h)
target_compile_definitions(${TARGET_NAME} PUBLIC -DTRILIBRARY)
target_compile_definitions(${TARGET_NAME} PUBLIC -DNO_TIMER)

if (MSVC)
  target_compile_definitions(${TARGET_NAME} PUBLIC -D_CRT_SECURE_NO_WARNINGS)
  target_compile_options(${TARGET_NAME} PUBLIC /wd4311 /wd4312 /wd4244 /wd4477)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(triangle triangle.c)
target_link_libraries(triangle trilib)
