cmake_minimum_required(VERSION 3.0)

project(triangle C)

set(TARGET_NAME triangle)

set(HDR_FILES
  construct.h
  debug.h
  delauny.h
  geom.h
  io.h
  locate.h
  mem.h
  meshqual.h
  meshtransform.h
  random.h
  stats.h
  triangle.h
  triangulate.h
  user.h
)

set(SRC_FILES
  construct.c
  debug.c
  delauny.c
  geom.c
  io.c
  locate.c
  mem.c
  meshqual.c
  meshtransform.c
  random.c
  stats.c
  triangulate.c
  user.c
)

add_library(${TARGET_NAME} STATIC ${HDR_FILES} ${SRC_FILES})
target_compile_definitions(${TARGET_NAME} PUBLIC -DTRILIBRARY)
target_compile_definitions(${TARGET_NAME} PUBLIC -DNO_TIMER)

if (MSVC)
  target_compile_definitions(${TARGET_NAME} PUBLIC -D_CRT_SECURE_NO_WARNINGS)
  target_compile_options(${TARGET_NAME} PUBLIC /wd4311 /wd4312 /wd4244 /wd4477)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
