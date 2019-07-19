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

# Find boost components
find_package(Boost COMPONENTS system thread REQUIRED)

set(SRC_FILES
  core/morfeusobject.cpp
  excitations/excitation.cpp
  excitations/planewave.cpp
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
  material/isotropicmaterial.cpp
  material/material.cpp
  math/functions.cpp
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
  observations/farfield.cpp
  observations/observation.cpp
  solver/solution.cpp
  solver/directsolver.cpp
  solver/iterativesolver.cpp
  solver/meshinformation.cpp
  solver/solver.cpp
)

set(HDR_FILES
  morfeus.h
  core/morfeusobject.h
  excitations/excitation.h
  excitations/planewave.h
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
  material/isotropicmaterial.h
  material/material.h
  math/constants.h
  math/functions.h
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
  observations/farfield.h
  observations/observation.h
  rapidxml/rapidxml.hpp
  rapidxml/rapidxml_iterators.hpp
  rapidxml/rapidxml_print.hpp
  rapidxml/rapidxml_utils.hpp
  rapidxml/xmlutils.h
  solver/solution.h
  solver/directsolver.h
  solver/iterativesolver.h
  solver/meshinformation.h
  solver/solver.h
)

# Create target
add_library(${TARGET_NAME} ${BUILD_TYPE} ${HDR_FILES} ${SRC_FILES})

# Set target compile options and definitions
if (MSVC)
  target_compile_definitions(${TARGET_NAME} PUBLIC -DBOOST_ALL_NO_LIB)
  target_compile_definitions(${TARGET_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(${TARGET_NAME} PUBLIC -D_USE_MATH_DEFINES)
  target_compile_options(${TARGET_NAME} PRIVATE /wd4311 /wd4312 /wd4244 /wd4477)
endif()
if (MORFEUS_BUILD_SHARED)
  target_compile_definitions(${TARGET_NAME} PRIVATE -DMORFEUS_COMPILE_LIBRARY)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/excitations)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/geometry)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/material)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/math)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesh)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesher)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/observations)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/solver)
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${TARGET_NAME} loki)
target_link_libraries(${TARGET_NAME} triangle)
target_link_libraries(${TARGET_NAME} bcrypt)
target_link_libraries(${TARGET_NAME} ${BOOST_LIBRARIES})
