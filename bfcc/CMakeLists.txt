cmake_minimum_required(VERSION 3.16)

project(bfcc)

file(GLOB_RECURSE FT_SRCS src/main.cpp src/bfstart.cpp src/output/outcoff.cpp)

add_executable(bfcc ${FT_SRCS})