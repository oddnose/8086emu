cmake_minimum_required(VERSION 3.10.0)
project(8086emu VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES 
    src/decoder.cpp
    src/decoder.h
		src/instruction-table.hpp
		src/instruction-table.cpp
)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()
add_library(8086emu ${SOURCE_FILES})

target_include_directories(8086emu PRIVATE src)
add_subdirectory(test)


