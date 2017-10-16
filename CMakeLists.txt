cmake_minimum_required(VERSION 3.8)
project(2521Ass2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1
		Graph.c
		graph.h
		url.h
		url.c
		pagerank.c)

add_executable(pagerank ${SOURCE_FILES_1})

set(SOURCE_FILES_2
		BSTree.c
		BSTree.h
		Graph.c
		graph.h
		url.h
		url.c
		inverted.c)

add_executable(inverted ${SOURCE_FILES_2})

set(SOURCE_FILES_4
		scaledFootrule.c)

add_executable(scaledFootrule ${SOURCE_FILES_4})

set(SOURCE_FILES_3
		url.c
		url.h
		Graph.c
		graph.h
		searchPagerank.c)

add_executable(searchPagerank ${SOURCE_FILES_3})