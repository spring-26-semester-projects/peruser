cmake_minimum_required(VERSION 3.15)

project(Peruser LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_library(OBJS OBJECT
    include/rpol.h
    include/ratomic.h
    src/rpol.C
    src/ratomic.C
)

target_include_directories(OBJS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(OBJS PRIVATE
        /std:c++20
        /W4
        /permissive-
        /TP # Treats all files as C++
    )
else() # GCC or Clang
    target_compile_options(OBJS PRIVATE
        -std=c++20
        -Wall
        -Wsign-compare
        -Wtype-limits
    )
endif()

target_compile_definitions(OBJS PUBLIC __DEBUG_BUILD)

# Primary Application Target
add_executable(peruser src/peruser.C)
target_link_libraries(peruser PRIVATE OBJS)
set_target_properties(peruser PROPERTIES LINKER_LANGUAGE CXX)

# Test Suite Binary Target
add_executable(tocc_test_runner tests/test_nfa.C)
target_link_libraries(tocc_test_runner PRIVATE OBJS)
set_target_properties(tocc_test_runner PROPERTIES LINKER_LANGUAGE CXX)