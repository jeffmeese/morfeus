cmake_minimum_required(VERSION 2.8)

project(morfeus C CXX)

set (CMAKE_CXX_STANDARD 11)

# Set output directory
if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Set default build type
if (NOT DEFINED CMAKE_BUILD_TYPE AND NOT MSVC_IDE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(src)
add_subdirectory(tools)
