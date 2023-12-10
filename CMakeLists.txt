cmake_minimum_required(VERSION 3.10)
project(memdb)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_library(memdb INTERFACE)
target_include_directories(memdb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(tests)
