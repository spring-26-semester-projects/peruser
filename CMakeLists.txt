cmake_minimum_required(VERSION 3.15)

project(Peruser LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_library(includ OBJECT
	include/rpol.h
	src/peruser.C
)

target_include_directories(includ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(includ PRIVATE -std=c++20 -g -Wall -Wsign-compare -Wtype-limits)

add_executable(peruser.elf)

target_link_libraries(peruser.elf PRIVATE includ)
set_target_properties(peruser.elf PROPERTIES
	LINKER_LANGUAGE CXX
)
