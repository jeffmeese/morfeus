cmake_minimum_required(VERSION 2.8)

project(morfeus C CXX)

if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(/wd4244 /wd4477)
endif()

add_subdirectory(triangle)
add_subdirectory(tetgen)
#add_subdirectory(loki)
