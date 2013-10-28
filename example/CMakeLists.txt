project(example)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
include_directories(${PROJECT_SOURCE_DIR}/../include)
add_executable(example ${PROJECT_SOURCE_DIR}/example.cc)
add_executable(fizzbuzz ${PROJECT_SOURCE_DIR}/fizzbuzz.cc)