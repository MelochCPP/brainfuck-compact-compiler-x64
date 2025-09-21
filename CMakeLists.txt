cmake_minimum_required(VERSION 3.31)
project(brainfuck)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "D:/brainfuck/dist")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "D:/brainfuck/dist")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "D:/brainfuck/dist")

add_subdirectory(bfcc)