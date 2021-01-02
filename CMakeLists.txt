cmake_minimum_required(VERSION 3.0)

project("LibBase64")

set(LIBRARY_NAME "base64")

include(CTest)

option(DEBUG "Debug build" OFF)
option(SHARED "Shared library" OFF)
if(NOT WIN32)
	option(STATIC "Static library" OFF)
	option(UNITTESTS "Build unittests" OFF)
endif()

if(${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
	set(CMAKE_CXX_COMPILER_ARG1 "-std=c++11" CACHE STRING "C++ version for eclipse" FORCE)
	set(CMAKE_ECLIPSE_VERSION "4.5" CACHE STRING "Eclipse version" FORCE)
endif()

if(UNITTESTS)
	add_subdirectory(test)
endif()

if(DEFINED DEBUG)
	# Turn on debug symbols
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Set compiler flags
set(CMAKE_CXX_FLAGS  "-O3 -Wall -Werror -std=c++11")

# Set linker flags
#SET( CMAKE_EXE_LINKER_FLAGS  "" )

set(sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/base64.cpp
)
set(includes
	${CMAKE_CURRENT_SOURCE_DIR}/inc/base64.hpp
)

if(SHARED)
	add_library(${LIBRARY_NAME} SHARED ${sources})
	target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_11)
	target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
	set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${includes}")

	if(NOT WIN32)
		# Install target
		install(
			TARGETS						${LIBRARY_NAME}
			LIBRARY DESTINATION			lib
			PUBLIC_HEADER DESTINATION	include
		)
	endif()
endif()

if(STATIC OR UNITTESTS)
add_library(base64_static STATIC ${sources})
target_include_directories(base64_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
endif()