// COMP2521: Assignment 2: Simple Search Engines
// Part-1: Graph structure-based Search Engine
// A: Calculate Weighted PageRanks
// Coded by George Bai/Eu Shaun Lim

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include <unistd.h>
#include "math.h"
#include "function.h"

// creates graph based on URLs in collection.txt
Graph buildGraph();

// get node of URL
int getID(char *string, char **list, int url_count);

// gets number of out-degrees of all URLs and stores them in an array
void getOutDegree(int url_count, int *out_degree);

// stores all Win values in a 2d array
int buildWin(double **Weights, int url_count, Graph G);

// stores all Wout values in a 2d array
int buildWout(double **Weights, int url_count, Graph G);

// gets specific Win value
double getWin(int i, int j, int url_count, Graph G);

// gets specific Wout value
double getWout(int i, int j, int url_count, Graph G);

// gets number of in-link to a URL
double getInLink(int i, int url_count, Graph G);

// gets number of out-link of a URL
double getOutLink(int i, int url_count, Graph G);

// sorts URLs and their out-degrees by pagerank values
void sort(char **url_list, int *out_count, double *pr, int size);

int main(int argc, char **argv) {

	if (argc != 4) {
		fprintf(stderr, "Not enough arguments!\n");
		return EXIT_FAILURE;
	}

	// convert arguments to double
	double d = atof(argv[1]);
	double diffPR = atof(argv[2]);
	double maxIt = atof(argv[3]);

	// get total number of URLs in collection.txt
	int url_count = getURLCount();
	// build graph based on URLs in collection.txt
	Graph g = buildGraph();

	// get Win values for all URLs
	double **Win = calloc(url_count, sizeof(double *));
	for (int i = 0; i < url_count; i++) {
		Win[i] = calloc(url_count, sizeof(double));
	}
	assert(Win != NULL);
	buildWin(Win, url_count, g);

	// get Wout values for all URLs
	double **Wout = calloc(url_count, sizeof(double *));
	for (int i = 0; i < url_count; i++) {
		Wout[i] = calloc(url_count, sizeof(double));
	}
	assert(Wout != NULL);
	buildWout(Wout, url_count, g);

	double pr[url_count];    // array containing page ranks of each url
	double prOld[url_count];    // old page ranks

	// get initial pagerank for each URLs
	int i;
	for (i = 0; i < url_count; i++) {
		prOld[i] = pr[i] = (double) 1 / url_count;
	}

	// get list of URLs
	char *url_list[url_count];
	getURLList(url_count, url_list);

	// pagerank algorithm
	int it = 0;
	double diff = diffPR;

	while (it < maxIt && diff >= diffPR) {
		it++;
		for (i = 0; i < url_count; i++) {
			int j;
			for (j = 0; j < url_count; j++) {
				prOld[j] = pr[j];	// copy to prOld before updating
			}
			double sum = 0;
			for (j = 0; j < url_count; j++) {
				if (edgeExists(g, j, i)) {	// if j is an outgoing link to i
					sum += pr[j] * Win[j][i] * Wout[j][i];
				}
			}
			pr[i] = (1 - d) / url_count + d * sum;

			// calculate difference between new and old pagerank value
			diff = 0.0;
			for (j = 0; j < url_count; j++) {
				diff += fabs(pr[i] - prOld[i]);
			}
		}
	}

	// get out degrees of URLs
	int *outDegree = calloc(url_count, sizeof(int));
	getOutDegree(url_count, outDegree);

	// sort by pagerank values and print to pagerankList.txt
	sort(url_list, outDegree, pr, url_count);
	FILE *fp = fopen("pagerankList.txt", "w");
	for (i = 0; i < url_count; i++) {
		fprintf(fp, "%s, %d, %.7lf\n", url_list[i], outDegree[i], pr[i]);
	}
	fclose(fp);

/*
	/////////// This is just printing out the Graph///////////////
	for (int i = 0; i < url_count; i++) {
		for (int j = 0; j < url_count; j++) {
			if(i==j) continue;
			if(Win[i][j] == 0.0 || Wout[i][j] == 0.0) continue;
			printf("Win[%d][%d]: %.7lf , Wout[%d][%d]: %.7lf \n", i, j, Win[i][j],  i, j, Wout[i][j]);
		}
		printf("\n");
	}
	//showGraph(g);
	/////////// This is just printing out the Graph///////////////
*/
	// free all memory
	free(Win);
	free(Wout);
	free(outDegree);
	dropGraph(g);
	return EXIT_SUCCESS;
}

// Builds Weighted Graph from collections.txt
Graph buildGraph() {

	// get number and list of URLs
	int url_count = getURLCount();
	Graph G = newGraph(url_count);
	char *url_list[url_count];
	getURLList(url_count, url_list);

	// open collection.txt to read
	FILE *fp = fopen("collection.txt", "r");
	char buffer[100], url[100];
	while (fscanf(fp, "%s", buffer) != EOF) {
		char temp_string[1000];
		//copy url in collection.txt and open it
		strcpy(temp_string, buffer);
		strcat(temp_string, ".txt");

		FILE *temp = fopen(temp_string, "r");
		int flag = 0;
		while (fscanf(temp, "%s", url) != EOF) {
			char section1[] = "Section-1";
			char end[] = "#end";

			if (strcmp(url, section1) == 0 && flag == 0) {
				flag = 1;	// set flag to 1 when in Section 1
				continue;
			}
			if (strcmp(url, end) == 0 && flag == 1) break;	// reached end of Section 1
			if (flag == 1) {	// means pointer is in Section 1
				if (strcmp(url, buffer) == 0) continue;    // ignore self-loops
				insertEdge(G, getID(buffer, url_list, url_count), getID(url, url_list, url_count), 1);
			}
		}
	}

	fclose(fp);
	return G;
}

// gets ID of string
int getID(char *string, char **list, int url_count) {
	int id = 0;
	while (id < url_count && strcmp(list[id], string)) {
		id++;
	}
	return id == url_count ? -1 : id;
}

// get number of out-degrees in each URL
// and store them in out_degree array
void getOutDegree(int url_count, int *out_degree) {
	FILE *fp;
	if ((fp = fopen("collection.txt", "r")) == NULL) {
		perror("collection.txt not found");
		return;
	}
	char buffer[100];
	char tmp_str[100];
	char url[100];
	int i = 0;
	// read collection.txt
	while (fscanf(fp, "%s", buffer) != EOF) {
		strcpy(tmp_str, buffer);
		strcat(tmp_str, ".txt");
		FILE *fp2;
		// open url*.txt in collection.txt
		if ((fp2 = fopen(tmp_str, "r")) == NULL) {
			perror("No such file");
			return;
		}
		while (fscanf(fp2, "%s", url) != EOF) {
			if (strcmp(url, buffer) == 0) continue;    // ignore self loops
			if (strcmp(url, "#end") == 0) {        // no more URLs to count
				i++;                            // move on to next URL
				break;
			}
			out_degree[i]++;
		}
		fclose(fp2);
	}
	for (i = 0; i < url_count; i++) {
		out_degree[i] -= 2;    //delete first 2 letters (#start Section-1)
	}
	fclose(fp);
}

// build weights for Win
int buildWin(double **Weights, int url_count, Graph G) {
	int i = 0;
	while (i < url_count) {
		int j = 0;
		while (j < url_count) {
			if (edgeExists(G, i, j)) {
				Weights[i][j] = getWin(i, j, url_count, G);
			}
			j++;
		}
		i++;
	}
	return 1;
}

// build weights for Wout
int buildWout(double **Weights, int url_count, Graph G) {
	int i = 0;
	while (i < url_count) {
		int j = 0;
		while (j < url_count) {
			if (edgeExists(G, i, j) == 1) {
				Weights[i][j] = getWout(i, j, url_count, G);
			}
			j++;
		}
		i++;
	}
	return 1;
}

// calculates Win
double getWin(int i, int j, int url_count, Graph G) {
	double weight = 0;

	double nom = getInLink(j, url_count, G);
	int x = 0;
	double denom = 0;
	while (x < url_count) {
		if (edgeExists(G, i, x)) {
			denom += getInLink(x, url_count, G);
		}
		x++;
	}

	weight = nom / denom;
	return weight;
}

// calculates Wout
double getWout(int i, int j, int url_count, Graph G) {
	double weight = 0;

	double nom = getOutLink(j, url_count, G);
	int x = 0;
	double denom = 0;
	while (x < url_count) {
		if (edgeExists(G, i, x) == 1) {
			denom += getOutLink(x, url_count, G);
		}
		x++;
	}

	weight = nom / denom;
	return weight;
}

// gets number of URLs pointing to i
double getInLink(int i, int url_count, Graph G) {
	int x = 0;
	double count = 0;
	while (x < url_count) {
		if (edgeExists(G, x, i) == 1) count++;
		x++;
	}
	return count;
}

// gets number of URLs pointing out of i
double getOutLink(int i, int url_count, Graph G) {
	int x = 0;
	double count = 0;
	while (x < url_count) {
		if (edgeExists(G, i, x) == 1) count++;
		x++;
	}
	if (count == 0) count = 0.5;
	return count;
}

// sort URLs using bubble sort
// modified from lab06
void sort(char **url_list, int *out_count, double *pr, int size) {
	int i, j, nswaps;
	for (i = 0; i < size; i++) {
		nswaps = 0;
		for (j = size - 1; j > i; j--) {
			if (pr[j] > pr[j - 1]) {
				char tmpstr[30];
				int tmp = out_count[j];
				double tmpPr = pr[j];
				strcpy(tmpstr, url_list[j]);

				// sort out_count
				out_count[j] = out_count[j - 1];
				out_count[j - 1] = tmp;

				// sort url_list
				strcpy(url_list[j], url_list[j - 1]);
				strcpy(url_list[j - 1], tmpstr);

				// sort pagerank
				pr[j] = pr[j - 1];
				pr[j - 1] = tmpPr;
				
				nswaps++;
			}
		}
		if (nswaps == 0) break;
	}
}
