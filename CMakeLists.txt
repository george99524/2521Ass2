cmake_minimum_required(VERSION 3.8)
project(ass2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Graph.c
        graph.h
        pagerank.c)

add_executable(ass2 ${SOURCE_FILES})