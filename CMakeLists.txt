# Minimum cmake version: 2.8
cmake_minimum_required(VERSION 2.8)

# Project name
project(Simple3DModelRenderer)

# Enable debug builds by default
if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Default Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -stdlib=libc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -stdlib=libc++")
	
# Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -pg")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -pg")

# Release Flags, no console on windows
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2")
if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mwindows")
endif(WIN32)

# RelWithDebInfo
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} -g")

# MinSizeRel, no console on windows
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -Os")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -Os")
if(WIN32)
	set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -mwindows")
	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -mwindows")
endif(WIN32)

# Requires OpenGL, SFML, and GLEW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML 2.1 REQUIRED system window)

# Source files in here
add_subdirectory(src)
