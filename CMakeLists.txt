cmake_minimum_required(VERSION 3.15)

project(Peruser LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_library(OBJS OBJECT
	include/rpol.h
	src/rpol.C
	src/peruser.C
)

target_include_directories(OBJS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
	target_compile_options(OBJS PRIVATE /W4 /std:c++20)
else()
	target_compile_options(OBJS PRIVATE -std=c++20 -g -Wall -Wsign-compare -Wtype-limits)
endif()

target_compile_definitions(OBJS PRIVATE __DEBUG_BUILD)

add_executable(peruser)

target_link_libraries(peruser PRIVATE OBJS)
set_target_properties(peruser PROPERTIES
	LINKER_LANGUAGE CXX
)
