cmake_minimum_required(VERSION 2.8)

project(morfeus CXX)

set(TARGET_NAME morfeus_console)

# Set policies
cmake_policy(SET CMP0020 NEW)

set(SRC_FILES
  application.cpp
  commandline.cpp
  main.cpp
)

set(HDR_FILES
  application.h
  commandline.h
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES})

if (MSVC)
  target_compile_definitions(${TARGET_NAME} PRIVATE -DMORFEUS_USE_LIBRARY)
endif()

target_link_libraries(${TARGET_NAME} morfeus)
