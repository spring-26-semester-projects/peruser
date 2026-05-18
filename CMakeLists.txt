cmake_minimum_required(VERSION 3.15)

project(Peruser LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(OBJS OBJECT
	include/rpol.h
	include/ratomic.h
	src/rpol.C
)

target_include_directories(OBJS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MSVC)
	target_compile_options(OBJS PUBLIC
		/W4
		/Zi
		/permissive-
	)
else()
	target_compile_options(OBJS PRIVATE
		-std=c++20
		-g
		-Wall
		-Wsign-compare
		-Wtype-limits
	)
endif()

target_compile_definitions(OBJS PUBLIC __DEBUG_BUILD)

add_executable(peruser
	src/peruser.C
	$<TARGET_OBJECTS:OBJS>
)

target_include_directories(peruser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(peruser PRIVATE OBJS)
set_target_properties(peruser PROPERTIES
	LINKER_LANGUAGE CXX
)

add_subdirectory(tests)
