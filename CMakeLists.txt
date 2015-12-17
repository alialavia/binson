cmake_minimum_required(VERSION 3.3)
project(binson)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        binsoncore.c
        binsoncore.h
        README.md jsonreader.c jsonreader.h binson.c)

add_executable(binson ${SOURCE_FILES})