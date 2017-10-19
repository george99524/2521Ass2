cmake_minimum_required(VERSION 3.8)
project(2521Ass2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1
		Graph.c
		graph.h
		function.c
		function.h
		pagerank.c)

add_executable(pagerank ${SOURCE_FILES_1})

set(SOURCE_FILES_2
		BSTree.c
		BSTree.h
		Graph.c
		graph.h
		function.c
		function.h
		inverted.c)

add_executable(inverted ${SOURCE_FILES_2})

set(SOURCE_FILES_3
		function.c
		function.h
		Graph.c
		graph.h
		searchPagerank.c)

add_executable(searchPagerank ${SOURCE_FILES_3})

set(SOURCE_FILES_4
		function.c
		function.h
		Graph.c
		graph.h
		searchTfIdf.c)

add_executable(searchTfIdf.c ${SOURCE_FILES_4})

set(SOURCE_FILES_5
		scaledFootrule.c)

add_executable(scaledFootrule ${SOURCE_FILES_5})