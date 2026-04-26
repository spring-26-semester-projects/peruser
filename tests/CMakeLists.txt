add_library(TESTSXX INTERFACE)

target_link_libraries(TESTSXX INTERFACE OBJS)
target_include_directories(TESTSXX INTERFACE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_options(TESTSXX INTERFACE -std=c++20 -g -Wall -Wsign-compare -Wtype-limits)

add_executable(peruser-tests
    regex-test.C
    $<TARGET_OBJECTS:OBJS>
)
target_link_libraries(peruser-tests PRIVATE TESTSXX OBJS)
set_target_properties(peruser-tests PROPERTIES LINKER_LANGUAGE CXX)
