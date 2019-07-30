cmake_minimum_required(VERSION 2.8)

project(morfeus C CXX)

# Define options
option(MORFEUS_BUILD_SHARED "Set to ON to build as a shared library" ON)

# Set target name
set(TARGET_NAME morfeus)

# Set policies
cmake_policy(SET CMP0020 NEW)

# Set build type
set(BUILD_TYPE STATIC)
if (MORFEUS_BUILD_SHARED)
  set(BUILD_TYPE SHARED)
endif()

# Add the 3rd party libraries
add_subdirectory(3rdparty)

# Find boost components
find_package(Boost COMPONENTS system thread REQUIRED)

set(SRC_FILES
  conditions/boundarycondition.cpp
  conditions/perfectelectricconductor.cpp
  core/morfeusobject.cpp
  excitations/excitation.cpp
  excitations/planewave.cpp
  excitations/probefeed.cpp
  geometry/box.cpp
  geometry/circle.cpp
  geometry/cylinder.cpp
  geometry/face.cpp
  geometry/hole.cpp
  geometry/model.cpp
  geometry/plane.cpp
  geometry/point2d.cpp
  geometry/point3d.cpp
  geometry/polygon.cpp
  geometry/rectangle.cpp
  geometry/region.cpp
  geometry/segment.cpp
  geometry/shape.cpp
  geometry/triangleshape.cpp
  geometry/vertex.cpp
  material/isotropicmaterial.cpp
  material/material.cpp
  material/materialdatabase.cpp
  math/functions.cpp
  math/gaussquad.cpp
  mesh/edge.cpp
  mesh/element.cpp
  mesh/face.cpp
  mesh/mesh.cpp
  mesh/node.cpp
  mesh/prismelement.cpp
  mesh/rectangleface.cpp
  mesh/tetelement.cpp
  mesh/triangleelement.cpp
  mesh/triangleface.cpp
  mesher/mesher.cpp
  mesher/meshrefinement.cpp
  observations/farfield.cpp
  observations/observation.cpp
  project/morfeusproject.cpp
  solution/solution.cpp
  solution/directsolver.cpp
  solution/iterativesolver.cpp
  solution/meshinformation.cpp
  solution/solver.cpp
)

set(HDR_FILES
  morfeus.h
  conditions/boundarycondition.h
  conditions/perfectelectricconductor.h
  core/morfeusobject.h
  core/factory.h
  excitations/excitation.h
  excitations/planewave.h
  excitations/probefeed.h
  geometry/box.h
  geometry/circle.h
  geometry/cylinder.h
  geometry/face.h
  geometry/hole.h
  geometry/model.h
  geometry/plane.h
  geometry/point2d.h
  geometry/point3d.h
  geometry/polygon.h
  geometry/rectangle.h
  geometry/region.h
  geometry/segment.h
  geometry/shape.h
  geometry/triangleshape.h
  geometry/vertex.h
  material/isotropicmaterial.h
  material/material.h
  material/materialdatabase.h
  math/constants.h
  math/functions.h
  math/gaussquad.h
  mesh/edge.h
  mesh/element.h
  mesh/face.h
  mesh/mesh.h
  mesh/node.h
  mesh/prismelement.h
  mesh/rectangleface.h
  mesh/tetelement.h
  mesh/triangleelement.h
  mesh/triangleface.h
  mesher/mesher.h
  mesher/meshrefinement.h
  observations/farfield.h
  observations/observation.h
  project/morfeusproject.h
  solution/solution.h
  solution/directsolver.h
  solution/iterativesolver.h
  solution/meshinformation.h
  solution/solver.h
  xml/rapidxml.hpp
  xml/rapidxml_iterators.hpp
  xml/rapidxml_print.hpp
  xml/rapidxml_utils.hpp
  xml/xmlutils.h
)

# Create target
add_library(${TARGET_NAME} ${BUILD_TYPE} ${HDR_FILES} ${SRC_FILES})

target_compile_definitions(${TARGET_NAME} PUBLIC -DTETLIBRARY)

# Set target compile options and definitions
if (MSVC)
  target_compile_definitions(${TARGET_NAME} PUBLIC -DBOOST_ALL_NO_LIB)
  target_compile_definitions(${TARGET_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(${TARGET_NAME} PRIVATE -D_SCL_SECURE_NO_WARNINGS)
  target_compile_definitions(${TARGET_NAME} PUBLIC -D_USE_MATH_DEFINES)
  target_compile_options(${TARGET_NAME} PRIVATE /wd4311 /wd4312 /wd4244 /wd4477)
endif()
if (MORFEUS_BUILD_SHARED)
  target_compile_definitions(${TARGET_NAME} PRIVATE -DMORFEUS_COMPILE_LIBRARY)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/conditions)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/excitations)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/geometry)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/material)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/math)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesh)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mesher)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/observations)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/solution)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/xml)
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

#target_link_libraries(${TARGET_NAME} loki)
target_link_libraries(${TARGET_NAME} tet)
target_link_libraries(${TARGET_NAME} libtri)
target_link_libraries(${TARGET_NAME} bcrypt)
target_link_libraries(${TARGET_NAME} ${BOOST_LIBRARIES})
