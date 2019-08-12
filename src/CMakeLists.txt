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
  model/model.cpp
  model/geometry/cuboid.cpp
  model/geometry/cylinder.cpp
  model/geometry/ellipse.cpp
  model/geometry/face.cpp
  model/geometry/facet.cpp
  model/geometry/hole.cpp
  model/geometry/line.cpp
  model/geometry/part.cpp
  model/geometry/part.cpp
  model/geometry/point.cpp
  model/geometry/polygon.cpp
  model/geometry/rectangle.cpp
  model/geometry/region.cpp
  model/geometry/segment.cpp
  model/geometry/vertex.cpp
  model/geometry/wire.cpp
  model/media/dielectric.cpp
  model/media/freespace.cpp
  model/media/medium.cpp
  model/media/perfectelectricconductor.cpp
  model/ports/port.cpp
  model/ports/wireport.cpp
  math/functions.cpp
  math/gaussquad.cpp
  mesh/convexhull.cpp
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
  sources/source.cpp
  sources/planewave.cpp
  sources/voltagesource.cpp
)

set(HDR_FILES
  morfeus.h
  core/factory.h
  core/morfeusobject.h
  core/morfeusproject.h
  model/model.h
  model/geometry/cuboid.h
  model/geometry/cylinder.h
  model/geometry/ellipse.h
  model/geometry/face.h
  model/geometry/facet.h
  model/geometry/hole.h
  model/geometry/line.h
  model/geometry/part.h
  model/geometry/point.h
  model/geometry/polygon.h
  model/geometry/rectangle.h
  model/geometry/region.h
  model/geometry/segment.h
  model/geometry/vertex.h
  model/geometry/wire.h
  model/media/dielectric.h
  model/media/freespace.h
  model/media/medium.h
  model/media/perfectelectricconductor.h
  model/ports/port.h
  model/ports/wireport.h
  math/constants.h
  math/functions.h
  math/gaussquad.h
  math/types.h
  mesh/convexhull.h
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
  sources/planewave.h
  sources/source.h
  sources/voltagesource.h
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
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/eigen)

#target_link_libraries(${TARGET_NAME} loki)
target_link_libraries(${TARGET_NAME} tet)
target_link_libraries(${TARGET_NAME} libtri)
target_link_libraries(${TARGET_NAME} bcrypt)
target_link_libraries(${TARGET_NAME} ${BOOST_LIBRARIES})
