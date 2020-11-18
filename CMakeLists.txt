cmake_minimum_required(VERSION 2.8)

project("LibBase64")

include(CTest)

option(STATIC_LIBRARY "Build static library" ON)
option(TESTS "Build unittests" ON)

if(${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
    set(CMAKE_CXX_COMPILER_ARG1 "-std=c++11" CACHE STRING "C++ version for eclipse" FORCE)
    set(CMAKE_ECLIPSE_VERSION "4.5" CACHE STRING "Eclipse version" FORCE)
endif()

if(DEFINED DEBUG)
	# Turn on debug symbols
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Set compiler flags
SET(CMAKE_CXX_FLAGS  "-O3 -Wall -Werror -std=c++11")
# Set linker flags
#SET( CMAKE_EXE_LINKER_FLAGS  "" )

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base64.cpp
)

add_library(libbase64 SHARED ${sources})
target_include_directories(libbase64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(STATIC_OPTION)
	add_library(libbase64_static STATIC tests.)
	target_include_directories(libbase64_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
endif()

if(TESTS_OPTION)
	add_subdirectory(test)
endif()
