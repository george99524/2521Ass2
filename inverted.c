// COMP2521: Assignment 2: Simple Search Engines
// Part-1: Graph structure-based Search Engine
// B: Inverted Index
// Coded by George Bai/Eu Shaun Lim

#include <stdlib.h>
#include <stdio.h>
#include "function.h"
#include "BSTree.h"
#include <string.h>

int main() {
	// get number of URLs in file
	int url_count = getURLCount();
	// get list of URLs
	char *url_list[url_count];
	getURLList(url_count, url_list);

	BSTree URLTree = newBSTree();
	FILE *fp;
	char words[1000];

	for (int i = 0; i < url_count; i++) {
		//temp string to store URL
		char tmp_url[30];
		strcpy(tmp_url, url_list[i]);
		strcat(tmp_url, ".txt");
		if ((fp = fopen(tmp_url, "r")) == NULL) {
			perror("No such file");
			return EXIT_FAILURE;
		}
		// start reading in words
		while (fscanf(fp, "%s", words) != EOF) {
			// skip the following words
			if (strcmp(words, "#start") == 0 ||
				strcmp(words, "#end") == 0 ||
				strcmp(words, "Section-1") == 0 ||
				strcmp(words, "Section-2") == 0 ||
				words[0] == 'u' ||
				words[1] == 'r' ||
				words[2] == 'l')
				continue;
			// normalise word before inserting
			normalise(words);
			// insert word into tree
			URLTree = BSTreeInsert(URLTree, words);
			// insert URL
			BSTreeInsertURL(URLTree, words, url_list[i]);
		}
		fclose(fp);
	}
	//showBSTree(URLTree);
	// print words and associated URLs into invertedIndex.txt
	FILE *fp1 = fopen("invertedIndex.txt", "w");
	BSTreePutInfix(URLTree, fp1);
	fclose(fp1);

	dropBSTree(URLTree);
	return EXIT_SUCCESS;
}