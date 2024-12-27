cmake_minimum_required(VERSION 3.29)
project(Network_Connection)

set(CMAKE_CXX_STANDARD 20)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
add_library(connections connection.cpp)
target_include_directories(connections PUBLIC ${CMAKE_SOURCE_DIR})
add_executable(Network_Connection main.cpp )
target_link_libraries(Network_Connection PRIVATE connections)

add_executable(test test.cpp connection.cpp)
