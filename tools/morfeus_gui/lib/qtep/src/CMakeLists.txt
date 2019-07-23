cmake_minimum_required(VERSION 2.8)

project(qtep CXX)

# Define options
option(QTEP_BUILD_SHARED "Set to ON to build as a shared library" OFF)

# Set policies
cmake_policy(SET CMP0020 NEW)

# Set target name
set(TARGET_NAME qtep)

# Set build type
set(BUILD_TYPE STATIC)
if (QTEP_BUILD_SHARED)
  set(BUILD_TYPE SHARED)
endif()

# Find Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Source files
set(SRC_FILES
  core/project.cpp
  core/projectitem.cpp
  gui/action.cpp
  gui/actionmanager.cpp
  gui/command.cpp
)

# Header files
set(HDR_FILES
  qtep.h
  core/project.h
  core/projectitem.h
  gui/action.h
  gui/actionmanager.h
  gui/command.h
)

# Create target
add_library(${TARGET_NAME} ${BUILD_TYPE} ${SRC_FILES} ${HDR_FILES})

# Set target compile options and definitions
if (QTEP_BUILD_SHARED)
  target_compile_definitions(${TARGET_NAME} PRIVATE -DQTEP_COMPILE_LIBRARY)
endif()

# Specify target include directories
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gui)

# Specify target link libraries
target_link_libraries(${TARGET_NAME} Qt5::Core)
target_link_libraries(${TARGET_NAME} Qt5::Gui)
target_link_libraries(${TARGET_NAME} Qt5::Widgets)
