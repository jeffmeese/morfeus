cmake_minimum_required(VERSION 3.0)

project(triangle C)

set(TARGET_NAME trilib)

set(HDR_FILES
    pool.h
    mem.h
    reduced.h
    cdt.h
    triangle.h
    io.h
    debug.h
    construct.h
    geom.h
    user.h
    init.h
    meshqual.h
    locate.h
    meshtransform.h
    primitives.h
    random.h
    delauny.h
    triangulate.h
)

set(SRC_FILES
    pool.c
    mem.c
    reduced.c
    cdt.c
    io.c
    debug.c
    construct.c
    geom.c
    user.c
    init.c
    meshqual.c
    locate.c
    meshtransform.c
    random.c
    delauny.c
    triangulate.c
)

add_library(${TARGET_NAME} STATIC ${HDR_FILES} ${SRC_FILES})
target_compile_definitions(${TARGET_NAME} PUBLIC -DTRILIBRARY)
target_compile_definitions(${TARGET_NAME} PUBLIC -DNO_TIMER)

if (MSVC)
  target_compile_definitions(${TARGET_NAME} PUBLIC -D_CRT_SECURE_NO_WARNINGS)
  target_compile_options(${TARGET_NAME} PUBLIC /wd4311 /wd4312 /wd4244 /wd4477)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(triangle triangle.c)
target_link_libraries(triangle trilib)
