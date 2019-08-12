cmake_minimum_required(VERSION 2.8)

project(morfeus CXX)

set(TARGET_NAME morfeus_gui)

# Set policies
cmake_policy(SET CMP0020 NEW)

# Find Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(lib)

# Find VTK and automatically initialize modules
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(SRC_FILES
  main.cpp
  3rdparty/ribbon/ribbon.cpp
  3rdparty/ribbon/ribbonbuttongroup.cpp
  3rdparty/ribbon/ribbontabcontent.cpp
  application/application.cpp
  application/mainwindow.cpp
  commands/addrectanglecommand.cpp
  commands/renamecommand.cpp
  dialogs/cubedialog.cpp
  dialogs/cylinderdialog.cpp
  project/guiproject.cpp
  project/meshitem.cpp
  project/rectangleitem.cpp
  project/surfaceitem.cpp
  views/detailsview.cpp
  views/view3d.cpp
  views/workspaceview.cpp
  workspacemodel/cubeitem.cpp
  workspacemodel/excitationsworkspaceitem.cpp
  workspacemodel/geometryworkspaceitem.cpp
  workspacemodel/geometrygroup.cpp
  workspacemodel/meshworkspaceitem.cpp
  workspacemodel/modelgroup.cpp
  workspacemodel/observationsworkspaceitem.cpp
  workspacemodel/partitem.cpp
  workspacemodel/rectangleworkspaceitem.cpp
  workspacemodel/workspacemodel.cpp
  workspacemodel/workspacemodelitem.cpp
)

set(HDR_FILES
  3rdparty/ribbon/ribbon.h
  3rdparty/ribbon/ribbonbuttongroup.h
  3rdparty/ribbon/ribbontabcontent.h
  application/application.h
  application/mainwindow.h
  commands/addrectanglecommand.h
  commands/renamecommand.h
  dialogs/cubedialog.h
  dialogs/cylinderdialog.h
  project/guiproject.h
  project/meshitem.h
  project/rectangleitem.h
  project/surfaceitem.h
  views/detailsview.h
  views/view3d.h
  views/workspaceview.h
  workspacemodel/cubeitem.h
  workspacemodel/excitationsworkspaceitem.h
  workspacemodel/geometryworkspaceitem.h
  workspacemodel/geometrygroup.h
  workspacemodel/meshworkspaceitem.h
  workspacemodel/modelgroup.h
  workspacemodel/observationsworkspaceitem.h
  workspacemodel/partitem.h
  workspacemodel/rectangleworkspaceitem.h
  workspacemodel/workspacemodel.h
  workspacemodel/workspacemodelitem.h
)

set(UI_FILES
  3rdparty/ribbon/ribbonbuttongroup.ui
  3rdparty/ribbon/ribbontabcontent.ui
  application/mainwindow.ui
  dialogs/cubedialog.ui
  dialogs/cylinderdialog.ui
)

set(RESOURCE_FILES
  resources/resources.qrc
)

add_executable(${TARGET_NAME} WIN32 ${HDR_FILES} ${SRC_FILES} ${UI_FILES} ${RESOURCE_FILES})

if (MSVC)
  target_compile_definitions(${TARGET_NAME} PRIVATE -DMORFEUS_USE_LIBRARY)
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/application)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/commands)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dialogs)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/project)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/views)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/workspacemodel)
target_include_directories(${TARGET_NAME} PRIVATE ${VTK_INCLUDE_DIRS})

target_link_libraries(${TARGET_NAME} morfeus)
target_link_libraries(${TARGET_NAME} qtep)
target_link_libraries(${TARGET_NAME} Qt5::Gui)
target_link_libraries(${TARGET_NAME} Qt5::Widgets)
target_link_libraries(${TARGET_NAME} ${VTK_LIBRARIES})
