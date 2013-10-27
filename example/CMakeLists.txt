project(example)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
include_directories(${PROJECT_SOURCE_DIR}/../include)
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
