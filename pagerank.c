#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include <unistd.h>
#include "math.h"
#include "pagerank.h"

// Builds Weighted Graph from collections.txt
Graph buildGraph() {

	int url_count = getURLCount();
	Graph G = newGraph(url_count);
	char *url_list[url_count];
	getURLList(url_count, url_list);

	FILE *fp;
	fp = fopen("collection.txt", "r");
	char buffer[100], url[100];
	while (fscanf(fp, "%s", buffer) != EOF) {
		char temp_string[1000];
		strcpy(temp_string, buffer);
		strcat(temp_string, ".txt");
		//printf("%s\n", buffer);

		FILE *temp = fopen(temp_string, "r");
		int flag = 0;
		while (fscanf(temp, "%s", url) != EOF) {
			char section1[] = "Section-1";
			char end[] = "#end";

			if (strcmp(url, section1) == 0) {
				if (flag == 0) {
					flag = 1;
					continue;
				}
			}
			if (strcmp(url, end) == 0 && flag == 1) {
				break;
			}

			if (flag == 1) {
				//printf("%s\n", url);
				//printf("%d\n", getID(url, url_list, url_count));
				insertEdge(G, getID(buffer, url_list, url_count), getID(url, url_list, url_count), 1);
			}
		}
		//printf("\n");
	}

	fclose(fp);
	return G;
}

int getID(char *string, char **list, int url_count) {
	int id = 0;
	while (id < url_count && strcmp(list[id], string)) {
		id++;
	}
	return id == url_count ? -1 : id;
}

int getURLCount() {
	FILE *fp;
	if ((fp = fopen("collection.txt", "r")) == NULL) {
		perror("No such file");
		return 0;
	}

	char url[100];
	int url_count = 0;
	while (fscanf(fp, "%s", url) != EOF) {
		url_count++;
		//printf("%s\n", url);
	}
	fclose(fp);

	return url_count;
}

void getURLList(int url_count, char **list) {
	int i = 0;
	FILE *fp;
	char url[100];

	fp = fopen("collection.txt", "r");
	while (i < url_count) {
		fscanf(fp, "%s", url);
		list[i] = malloc(100);
		strcpy(list[i], url);
		i++;
	}
}

void getOutCount(int url_count, int *out_count) {
	FILE *fp;
	if ((fp = fopen("collection.txt", "r")) == NULL) {
		perror("No such file");
		return;
	}

	char tmp_str[100];
	char url[100];
	int i = 0;

	while (fscanf(fp, "%s", tmp_str) != EOF) {
		strcat(tmp_str, ".txt");
		FILE *fp2;
		if ((fp2 = fopen(tmp_str, "r")) == NULL) {
			perror("No such file");
			return;
		}
		while (fscanf(fp2, "%s", url) != EOF) {
			if (strcmp(url, "#end") == 0) {
				i++;
				break;
			}
			out_count[i]++;
		}
	}
	for (i = 0; i < url_count; i++) {
		out_count[i] -= 2;    //delete first 2 letters (#start Section-1)
	}
	fclose(fp);
}

void PageRankW(Graph g, double d, double diffPR, double maxIt) {
	int n = getURLCount();
	double pr[n];    // array containing page ranks of each url
	double prOld[n];    // old page ranks

	for (int i = 0; i < n; i++) {
		pr[i] = (double) 1 / n;
		prOld[i] = pr[i];
	}
	int it = 0;
	double diff = diffPR;
	while (it < maxIt && diff >= diffPR) {
		for (int i = 0; i < n; i++) {

			/*
			 * pr[i] = (1 - d) / n + d;
			 */

			double tmp = fabs(pr[i] - prOld[i]);
			diff += tmp;
		}
		it++;
	}
}

int main(int argc, char **argv) {

	if (argc != 4) {
		fprintf(stderr, "Not enough arguments");
		return EXIT_FAILURE;
	}

	double d = atof(argv[1]);
	double diffPR = atof(argv[2]);
	double maxIt = atof(argv[3]);

	int url_count = getURLCount();
	Graph g = buildGraph();
	PageRankW(g, d, diffPR, maxIt);

	int outCount[url_count];
	for (int i = 0; i < url_count; i++) {
		outCount[i] = 0;
	}
	getOutCount(url_count, outCount);

	char *url_list[url_count];
	getURLList(url_count, url_list);

	FILE *fp = fopen("pagerankList.txt", "w");
	for (int i = 0; i < url_count; i++) {
		//printf("%d ", outCount[i]);
		fprintf(fp, "%s, %d\n", url_list[i], outCount[i]);
	}

	return EXIT_SUCCESS;
}

