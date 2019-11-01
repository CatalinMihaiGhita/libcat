cmake_minimum_required(VERSION 3.0.2)
project(libcat)
add_definitions(-std=c++17 -O3 -Werror)
include_directories(include)

add_subdirectory(tests)
