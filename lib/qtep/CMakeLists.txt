cmake_minimum_required(VERSION 2.8)

project(qtep CXX)

# Set default build type
if (NOT DEFINED CMAKE_BUILD_TYPE AND NOT MSVC_IDE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(3rdparty/loki)
add_subdirectory(src)
