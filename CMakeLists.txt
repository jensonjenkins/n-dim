cmake_minimum_required(VERSION "3.10")
project("ndim")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_executable(array_test tests/array_test.cpp)
add_executable(iterator_test tests/iterator_test.cpp)
add_executable(static_dim_test tests/static_dim_test.cpp)

