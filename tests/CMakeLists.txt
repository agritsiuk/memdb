find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-ggdb3 -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

add_executable(MemDbTest MemDbTest.cpp)
target_compile_options(MemDbTest PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak)
target_link_libraries(MemDbTest PRIVATE memdb gtest gtest_main asan ubsan)
add_test(NAME MemDbTest COMMAND MemDbTest)
