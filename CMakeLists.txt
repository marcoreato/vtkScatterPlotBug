cmake_minimum_required(VERSION 2.8)

PROJECT(Plot)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -ggdb3")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(Plot Plot.cpp)

target_link_libraries(Plot ${VTK_LIBRARIES})