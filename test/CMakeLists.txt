include(${PROJECT_SOURCE_DIR}/cmake/modules/gtest.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src)
set(TEST_FILES 
    ${CMAKE_SOURCE_DIR}/src/decoder.h
    ${CMAKE_SOURCE_DIR}/src/decoder.cpp
    ${CMAKE_SOURCE_DIR}/src/instruction-table.hpp
    ${CMAKE_SOURCE_DIR}/src/instruction-table.cpp
    src/test_nasm_decoder.cpp
    src/test_nasm_simulator.cpp
)

add_executable(8086emu_test ${TEST_FILES})


target_link_libraries(
    8086emu_test PRIVATE 
    GTest::gtest_main
		8086emu_lib
)

target_include_directories(8086emu_test PRIVATE src)
target_include_directories(8086emu_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
file(COPY ${CMAKE_SOURCE_DIR}/test/res DESTINATION ${CMAKE_BINARY_DIR}/test)


include(GoogleTest)
gtest_discover_tests(8086emu_test)

set_property(TARGET 8086emu_test PROPERTY WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test)
