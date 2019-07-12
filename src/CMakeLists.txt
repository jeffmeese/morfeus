cmake_minimum_required(VERSION 2.8)

project(morfeus C CXX)

# Define options
option(MORFEUS_BUILD_SHARED "Set to ON to build as a shared library" OFF)

# Set target name
set(TARGET_NAME morfeus)

# Set policies
cmake_policy(SET CMP0020 NEW)

# Set build type
set(BUILD_TYPE STATIC)
if (MORFEUS_BUILD_SHARED)
  set(BUILD_TYPE SHARED)
endif()

set(SRC_FILES
  excitations/excitation.cpp
  geometry/circle.cpp
  geometry/geometry.cpp
  geometry/hole.cpp
  geometry/point2d.cpp
  geometry/polygon.cpp
  geometry/rectangle.cpp
  geometry/region.cpp
  geometry/segment.cpp
  geometry/shape.cpp
  geometry/triangleshape.cpp
  geometry/vertex.cpp
  math/matrix3x3.cpp
  math/sparsematrix.cpp
  mesh/edge.cpp
  mesh/element.cpp
  mesh/face.cpp
  mesh/mesh.cpp
  mesh/node.cpp
  mesh/prismelement.cpp
  mesh/rectangleface.cpp
  mesh/triangleelement.cpp
  mesh/triangleface.cpp
  mesher/mesher.cpp
  mesher/meshrefinement.cpp
  observations/observation.cpp
  solver/solution.cpp
  solver/solver.cpp
)

set(HDR_FILES
  morfeus.h
  excitations/excitation.h
  geometry/circle.h
  geometry/geometry.h
  geometry/hole.h
  geometry/point2d.h
  geometry/polygon.h
  geometry/rectangle.h
  geometry/region.h
  geometry/segment.h
  geometry/shape.h
  geometry/triangleshape.h
  geometry/vertex.h
  math/matrix3x3.h
  math/sparsematrix.h
  mesh/edge.h
  mesh/element.h
  mesh/face.h
  mesh/mesh.h
  mesh/node.h
  mesh/prismelement.h
  mesh/rectangleface.h
  mesh/triangleelement.h
  mesh/triangleface.h
  mesher/mesher.h
  mesher/meshrefinement.h
  observations/observation.h
  solver/solution.h
  solver/solver.h
)

# Create target
add_library(${TARGET_NAME} ${BUILD_TYPE} ${HDR_FILES} ${SRC_FILES})

# Set target compile options and definitions
target_compile_definitions(${TARGET_NAME} PUBLIC -DTRILIBRARY)
target_compile_definitions(${TARGET_NAME} PUBLIC -DNO_TIMER)
target_compile_definitions(${TARGET_NAME} PUBLIC -DANSI_DECLARATORS)
if (MSVC)
  target_compile_definitions(${TARGET_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(${TARGET_NAME} PUBLIC -D_USE_MATH_DEFINES)
  target_compile_options(${TARGET_NAME} PRIVATE /wd4311 /wd4312 /wd4244 /wd4477)
endif()
if (MORFEUS_BUILD_SHARED)
  target_compile_definitions(${TARGET_NAME} PRIVATE -DMORFEUS_COMPILE_LIBRARY)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/excitations)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/geometry)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/math)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesh)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesher)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/observations)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/solver)

target_link_libraries(${TARGET_NAME} loki)
target_link_libraries(${TARGET_NAME} triangle)
