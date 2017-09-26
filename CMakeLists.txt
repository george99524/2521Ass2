cmake_minimum_required(VERSION 3.8)
project(2521Ass2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1
		Graph.c
		graph.h
		pagerank.c
		pagerank.h)

add_executable(pagerank ${SOURCE_FILES_1})

