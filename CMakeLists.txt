cmake_minimum_required(VERSION 2.8)
project(vjson)
add_definitions(-Wall -Wextra
	-Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align
	-Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual
	-Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code)	
add_executable(vjson_test block_allocator.cpp json.cpp main.cpp)
