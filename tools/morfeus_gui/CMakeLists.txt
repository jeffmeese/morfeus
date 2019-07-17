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
  application/application.cpp
  application/mainwindow.cpp
  commands/addrectanglecommand.cpp
  commands/renamecommand.cpp
  project/guiproject.cpp
  project/meshitem.cpp
  project/rectangleitem.cpp
  project/surfaceitem.cpp
  workspacemodel/excitationsworkspaceitem.cpp
  workspacemodel/geometryworkspaceitem.cpp
  workspacemodel/meshworkspaceitem.cpp
  workspacemodel/observationsworkspaceitem.cpp
  workspacemodel/rectangleworkspaceitem.cpp
  workspacemodel/workspacemodel.cpp
  workspacemodel/workspacemodelitem.cpp
  workspaceview/workspaceview.cpp
)

set(HDR_FILES
  application/application.h
  application/mainwindow.h
  commands/addrectanglecommand.h
  commands/renamecommand.h
  project/guiproject.h
  project/meshitem.h
  project/rectangleitem.h
  project/surfaceitem.h
  workspacemodel/excitationsworkspaceitem.h
  workspacemodel/geometryworkspaceitem.h
  workspacemodel/meshworkspaceitem.h
  workspacemodel/observationsworkspaceitem.h
  workspacemodel/rectangleworkspaceitem.h
  workspacemodel/workspacemodel.h
  workspacemodel/workspacemodelitem.h
  workspaceview/workspaceview.h
)

set(UI_FILES
  application/mainwindow.ui
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES} ${UI_FILES})

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/application)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/commands)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/project)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/workspacemodel)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/workspaceview)
target_include_directories(${TARGET_NAME} PRIVATE ${VTK_INCLUDE_DIRS})

target_link_libraries(${TARGET_NAME} morfeus)
target_link_libraries(${TARGET_NAME} qtep)
target_link_libraries(${TARGET_NAME} Qt5::Gui)
target_link_libraries(${TARGET_NAME} Qt5::Widgets)
target_link_libraries(${TARGET_NAME} ${VTK_LIBRARIES})
