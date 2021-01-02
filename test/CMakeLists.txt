cmake_minimum_required(VERSION 2.8)

set(sources ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

add_executable(unittest ${sources})
target_link_libraries(unittest LINK_PUBLIC base64_static CppUTest)
add_dependencies(unittest base64_static)

enable_testing()
add_test(NAME Base64Group COMMAND unittest -v -g Base64Group)

add_custom_command(
	TARGET unittest
	COMMENT "Running unit tests"
	POST_BUILD
	COMMAND ctest -C $<CONFIGURATION> --output-on-failure
)
