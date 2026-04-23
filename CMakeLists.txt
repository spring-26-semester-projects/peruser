cmake_minimum_required(VERSION 3.15)

project(Peruser LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_library(OBJS OBJECT
	include/rpol.h
	src/rpol.C
)

target_include_directories(OBJS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(OBJS PRIVATE
	-std=c++20
	-g
	-Wall
	-Wsign-compare
	-Wtype-limits
)

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

add_library(TESTSXX OBJECT
	tests/basic-regex.h
	tests/_util.h
	tests/valid-symbols.h	
)

target_link_libraries(TESTSXX PUBLIC OBJS)

target_include_directories(TESTSXX PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/tests)

target_compile_options(TESTSXX PRIVATE
	-std=c++20
	-g
	-Wall
	-Wsign-compare
	-Wtype-limits
)

add_executable(peruser-tests
	tests/regex-test.C
	$<TARGET_OBJECTS:TESTSXX>
	$<TARGET_OBJECTS:OBJS>
)

target_link_libraries(peruser-tests PRIVATE TESTSXX OBJS)
set_target_properties(peruser-tests PROPERTIES
	LINKER_LANGUAGE CXX
)
