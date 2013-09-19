project(test)
cmake_minimum_required(VERSION 2.8)

# amulet requres C++1y
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

# test sources
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)

# amulet
include_directories(${PROJECT_SOURCE_DIR}/../include)

# gtest
include_directories(${PROJECT_SOURCE_DIR}/gtest/include)
include_directories(${PROJECT_SOURCE_DIR}/gtest)
list(APPEND SRC_LIST ${PROJECT_SOURCE_DIR}/gtest/src/gtest-all.cc)

add_executable(${PROJECT_NAME} ${SRC_LIST})
