include(${PROJECT_SOURCE_DIR}/cmake/modules/gtest.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src)
set(TEST_FILES 
    ${CMAKE_SOURCE_DIR}/src/decoder.h
    ${CMAKE_SOURCE_DIR}/src/decoder.cpp
    src/test_decoder.cpp
)

add_executable(8086emu_test ${TEST_FILES})


target_link_libraries(
    8086emu_test PRIVATE 
    GTest::gtest_main
)

target_include_directories(8086emu_test PRIVATE src)
file(COPY ${CMAKE_SOURCE_DIR}/test/res DESTINATION ${CMAKE_BINARY_DIR}/test)
file(COPY ${CMAKE_SOURCE_DIR}/test/res DESTINATION ${CMAKE_BINARY_DIR})

include(GoogleTest)
gtest_discover_tests(8086emu_test)