cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME loki)
project(${PROJECT_NAME} C CXX)

# Set target name
set(TARGET_NAME ${PROJECT_NAME})

# Configure options
# Currently, shared build is not implemented
#option(LOKI_BUILD_SHARED "Set to ON to build loki as a shared library" OFF)

# Set version information
# TODO: Should set this from a file in the source tree
set(LOKI_VERSION_MAJOR 0)
set(LOKI_VERSION_MINOR 1)
set(LOKI_VERSION_PATCH 7)
set(LOKI_VERSION ${LOKI_VERSION_MAJOR}.${LOKI_VERSION_MINOR}.${LOKI_VERSION_PATCH})
message(STATUS "${PROJECT_NAME} version=${LOKI_VERSION}")

# Set the build type
set(LOKI_BUILD_TYPE STATIC)
if (LOKI_BUILD_SHARED)
  set(LOKI_BUILD_TYPE SHARED)
endif()
message(STATUS "LOKI_BUILD_TYPE = ${LOKI_BUILD_TYPE}")

# Set include current directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure build configuration(s)
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set flags for Windows build
if (MSVC AND NOT MINGW)
  add_definitions(/wd4251 /wd4267)
  add_definitions(/D_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
endif()

# Set flags for linux build
if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-literal-suffix")
endif()

# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Source files
set(SOURCE_FILES
	LevelMutex.cpp
	OrderedStatic.cpp
	SafeFormat.cpp
	Singleton.cpp
	SmallObj.cpp
	SmartPtr.cpp
	StrongPtr.cpp
)

# Header files
set(HEADER_FILES
	AbstractFactory.h
	Allocator.h
	AssocVector.h
	CachedFactory.h
	Checker.h
	CheckReturn.h
	ConstPolicy.h
	DataGenerators.h
	EmptyType.h
	Factory.h
	Function.h
	Functor.h
	HierarchyGenerators.h
	Key.h
	LevelMutex.h
	LockingPtr.h
	LokiExport.h
	LokiTypeInfo.h
	MultiMethods.h
	NullType.h
	OrderedStatic.h
	Pimpl.h
	RefToValue.h
	Register.h
	SafeBits.h
	SafeFormat.h
	ScopeGuard.h
	Sequence.h
	Singleton.h
	SmallObj.h
	SmartPtr.h
	SPCachedFactory.h
	static_check.h
	StrongPtr.h
	Threads.h
	Tuple.h
	Typelist.h
	TypelistMacros.h
	TypeManip.h
	TypeTraits.h
	Visitor.h
	flex/allocatorstringstorage.h
	flex/cowstringopt.h
	flex/flex_string.h
	flex/flex_string_details.h
	flex/flex_string_shell.h
	flex/simplestringstorage.h
	flex/smallstringopt.h
	flex/vectorstringstorage.h
	yasli/platform.h
	yasli/random.h
	yasli/yasli_fill_iterator.h
	yasli/yasli_memory.h
	yasli/yasli_protocols.h
	yasli/yasli_traits.h
	yasli/yasli_vector.h
)

# Create target
add_library(${TARGET_NAME} ${LOKI_BUILD_TYPE} ${SOURCE_FILES} ${HEADER_FILES})

target_compile_definitions(${TARGET_NAME} PUBLIC -DLOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT)

# Set target include directories
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Specify installation
set(INCLUDE_DIRECTORY "${CMAKE_INSTALL_PREFIX}/include")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${INCLUDE_DIRECTORY} FILES_MATCHING PATTERN *.h)
if (LOKI_BUILD_SHARED)
  install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
else()
  install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
endif()
