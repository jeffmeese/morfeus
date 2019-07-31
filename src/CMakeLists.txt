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
  core/morfeusobject.cpp
  core/morfeusproject.cpp
  excitations/excitation.cpp
  excitations/planewave.cpp
  excitations/probefeed.cpp
  geometry/cuboid.cpp
  geometry/cylinder.cpp
  geometry/ellipse.cpp
  geometry/face.cpp
  geometry/hole.cpp
  geometry/model.cpp
  geometry/part.cpp
  geometry/point.cpp
  geometry/polygon.cpp
  geometry/rectangle.cpp
  geometry/region.cpp
  geometry/segment.cpp
  geometry/solid.cpp
  geometry/surface.cpp
  geometry/vertex.cpp
  math/functions.cpp
  math/gaussquad.cpp
  media/dielectricmedium.cpp
  media/medium.cpp
  media/medialibrary.cpp
  media/perfectelectricconductor.cpp
  mesh/edge.cpp
  mesh/element.cpp
  mesh/face.cpp
  mesh/mesh.cpp
  mesh/node.cpp
  mesh/prism.cpp
  mesh/rectangle.cpp
  mesh/tetrahedron.cpp
  mesh/triangle.cpp
  mesher/mesher.cpp
  observations/farfield.cpp
  observations/observation.cpp
  solution/solution.cpp
  solution/directsolver.cpp
  solution/iterativesolver.cpp
  solution/meshinformation.cpp
  solution/solver.cpp
)

set(HDR_FILES
  morfeus.h
  core/factory.h
  core/morfeusobject.h
  core/morfeusproject.h
  excitations/excitation.h
  excitations/planewave.h
  excitations/probefeed.h
  geometry/cuboid.h
  geometry/cylinder.h
  geometry/ellipse.h
  geometry/face.h
  geometry/hole.h
  geometry/model.h
  geometry/part.h
  geometry/point.h
  geometry/polygon.h
  geometry/rectangle.h
  geometry/region.h
  geometry/segment.h
  geometry/solid.h
  geometry/surface.h
  geometry/vertex.h
  math/constants.h
  math/functions.h
  math/gaussquad.h
  media/dielectricmedium.h
  media/medium.h
  media/medialibrary.h
  media/perfectelectricconductor.h
  mesh/edge.h
  mesh/element.h
  mesh/face.h
  mesh/mesh.h
  mesh/node.h
  mesh/prism.h
  mesh/rectangle.h
  mesh/tetrahedron.h
  mesh/triangle.h
  mesher/mesher.h
  observations/farfield.h
  observations/observation.h
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
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

#target_link_libraries(${TARGET_NAME} loki)
target_link_libraries(${TARGET_NAME} tet)
target_link_libraries(${TARGET_NAME} libtri)
target_link_libraries(${TARGET_NAME} bcrypt)
target_link_libraries(${TARGET_NAME} ${BOOST_LIBRARIES})
