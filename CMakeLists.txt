cmake_minimum_required(VERSION 3.8)
project(2521Ass2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1
		Graph.c
		graph.h
		pagerank.c
		url.h
		url.c)

add_executable(pagerank ${SOURCE_FILES_1})

set(SOURCE_FILES_2
		BSTree.c
		BSTree.h
		Graph.c
		graph.h
		inverted.c
		inverted.h
		url.h
		url.c)

add_executable(inverted ${SOURCE_FILES_2})