cmake_minimum_required(VERSION 3.26)
project(holdr C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CTest)

include_directories(src)
include_directories(external)

add_subdirectory (src) 
add_subdirectory (tests)

enable_testing()
