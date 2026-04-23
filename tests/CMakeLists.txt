cmake_minimum_required(VERSION 3.15)

set(Unit_Tests "Unit Tests")

set(TESTS_OBJS
	_util.h
	valid-symbols.h
	regex-test.C
)

add_library(Unit_Tests STATIC ${TESTS_OBJS})

target_include_directories(Unit_Tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

target_compile_options(Unit_Tests PRIVATE
	-std=c++20
	-g
	-Wall
	-Wsign-compare
	-Wtype-limits
)
