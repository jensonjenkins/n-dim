cmake_minimum_required(VERSION "3.10")
project("ndim")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_executable(array_test tests/array_test.cpp)
add_executable(iterator_test tests/iterator_test.cpp)

