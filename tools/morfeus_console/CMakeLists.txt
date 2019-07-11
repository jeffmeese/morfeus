cmake_minimum_required(VERSION 2.8)

project(morfeus CXX)

set(TARGET_NAME morfeus_console)

# Set policies
cmake_policy(SET CMP0020 NEW)

set(SRC_FILES
  application.cpp
  inputdata.cpp
  main.cpp
)

set(HDR_FILES
  application.h
  inputdata.h
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES})

target_link_libraries(${TARGET_NAME} morfeus)
