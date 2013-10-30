cmake_minimum_required(VERSION 2.8)
project(vjson)
add_definitions(-Wall -Wextra -Wcast-qual)
add_executable(vjson_test block_allocator.cpp json.cpp main.cpp)
