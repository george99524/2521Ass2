// COMP2521: Assignment 2: Simple Search Engines
// Part-1: Graph structure-based Search Engine
// C: Search Engine
// Coded by George Bai/Eu Shaun Lim

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "function.h"

void sortURL(char **urlList, int *urlCounter, int size);

void sortPageRank(char **urlList, int *urlCounter, double *pagerankURL, int size);

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Give me at least one key word");
		return EXIT_FAILURE;
	}
	int numWords = 0;
	char *keyWords[argc - 1];
	// copy arguments to array of string
	for (int i = 0; i < argc - 1; i++) {
		keyWords[i] = argv[i + 1];
		normalise(keyWords[i]);
		numWords++;
	}
	// open invertedIndex.txt
	FILE *invertedFp;
	if ((invertedFp = fopen("invertedIndex.txt", "r")) == NULL) {
		fprintf(stderr, "invertedIndex.txt not found");
		return EXIT_FAILURE;
	}
	char words[100];
	// get total number of URLs in collection.txt
	int n = getURLCount();
	char *urlList[n];
	int urlCounter[n];
	// set counter to 0
	int i;
	for (i = 0; i < n; i++) {
		urlCounter[i] = 0;
	}
	int size = 0;
	i = 0;
	while (fscanf(invertedFp, "%s", words) != EOF) {
		// reached the line of the word
		if (i < numWords && strcmp(words, keyWords[i]) == 0) {
			char url[20];
			// read URLs associated with word
			while (fscanf(invertedFp, "%s", url) != EOF &&
				   url[0] == 'u' &&
				   url[1] == 'r' &&
				   url[2] == 'l') {
				// check if url has already been added to list
				// increment counter if yes
				int j;
				for (j = 0; j < size; j++) {
					if (strcmp(urlList[j], url) == 0) {
						urlCounter[j]++;
						break;
					}
				}
				// add url to list if not
				if (j == size) {
					urlList[size] = strdup(url);
					urlCounter[size]++;
					size++;
				}
			}
			i++;
			rewind(invertedFp);
		}
	}
	// sort URL by counter
	sortURL(urlList, urlCounter, size);
	fclose(invertedFp);

	// open pagerankList.txt
	FILE *pagerankFp;
	if ((pagerankFp = fopen("pagerankList.txt", "r")) == NULL) {
		fprintf(stderr, "pagerankList.txt not found");
		return EXIT_FAILURE;
	}
	// array to store pagerank values
	double pagerankURL[size];
	char url[30];
	char tmpurl[30];
	int x;
	double pagerank;
	i = 0;
	while (fscanf(pagerankFp, "%s %d %lf\n", url, &x, &pagerank) != EOF) {
		// fscanf reads url and x at every even times
		// but reads pagerank at every odd times
		if (i % 2 == 0) {
			normalise(url);
			strcpy(tmpurl, url);
		} else {
			for (int j = 0; j < size; j++) {
				if (strcmp(tmpurl, urlList[j]) == 0)
					pagerankURL[j] = pagerank;
			}
		}
		i++;
	}
	fclose(pagerankFp);

	// sort URL by pagerank
	sortPageRank(urlList, urlCounter, pagerankURL, size);
	// print urlList
	if (size < 30) {
		for (int j = 0; j < size; j++) {
			printf("%s\n", urlList[j]);
		}
	} else {
		for (int j = 0; j < 30; j++) {
			printf("%s\n", urlList[j]);
		}
	}
/*
	printf("\n");
	for (int j = 0; j < size; j++) {
		printf("%s     ", urlList[j]);
	}
	printf("\n");
	for (int j = 0; j < size; j++) {
		printf("%d         ", urlCounter[j]);
	}
	printf("\n");
	for (int j = 0; j < size; j++) {
		printf("%.7lf ", pagerankURL[j]);
	}
*/

	return EXIT_SUCCESS;
}

// sort URLs using bubble sort
// modified from lab06
void sortURL(char **urlList, int *urlCounter, int size) {
	int i, j, nswaps;
	for (i = 0; i < size; i++) {
		nswaps = 0;
		for (j = size - 1; j > i; j--) {
			if (urlCounter[j] > urlCounter[j - 1]) {
				char tmpstr[30];
				int tmp = urlCounter[j];
				strcpy(tmpstr, urlList[j]);
				// sort urlCounter
				urlCounter[j] = urlCounter[j - 1];
				urlCounter[j - 1] = tmp;
				// sort urlList
				strcpy(urlList[j], urlList[j - 1]);
				strcpy(urlList[j - 1], tmpstr);
				nswaps++;
			}
		}
		if (nswaps == 0) break;
	}
}

// actual sorting for sortPageRank here
void sortPR(char **urlList, double *pagerankURL, int lCurs, int rCurs) {
	int nswaps;
	int size = rCurs;
	for (; lCurs < size; lCurs++) {
		nswaps = 0;
		for (; rCurs > lCurs; rCurs--) {
			if (pagerankURL[rCurs] > pagerankURL[rCurs - 1]) {
				char tmpstr[30];
				double tmp = pagerankURL[rCurs];
				strcpy(tmpstr, urlList[rCurs]);
				// sort pagerankURL
				pagerankURL[rCurs] = pagerankURL[rCurs - 1];
				pagerankURL[rCurs - 1] = tmp;
				// sort urlList
				strcpy(urlList[rCurs], urlList[rCurs - 1]);
				strcpy(urlList[rCurs - 1], tmpstr);
				nswaps++;
			}
		}
		if (nswaps == 0) break;
	}
}

// sort URLs by pagerank values
void sortPageRank(char **urlList, int *urlCounter, double *pagerankURL, int size) {
	int lCurs = 0;
	int rCurs = 1;
	while (lCurs < size) {
		if (urlCounter[lCurs] == urlCounter[rCurs]) {
			sortPR(urlList, pagerankURL, lCurs, rCurs);
			rCurs++;
		} else {
			while (lCurs != rCurs) lCurs++;
			rCurs = lCurs + 1;
		}
	}
}
