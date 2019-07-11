cmake_minimum_required(VERSION 2.8)

project(morfeus CXX)

set(TARGET_NAME morfeus_gui)

# Set policies
cmake_policy(SET CMP0020 NEW)

# Find Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Find VTK and automatically initialize modules
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(SRC_FILES
    main.cpp
)

set(HDR_FILES
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES})

target_link_libraries(${TARGET_NAME} morfeus)
target_link_libraries(${TARGET_NAME} Qt5::Gui)
target_link_libraries(${TARGET_NAME} Qt5::Widgets)
target_link_libraries(${TARGET_NAME} ${VTK_LIBRARIES})
