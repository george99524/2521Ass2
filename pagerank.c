#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include <unistd.h>
#include "math.h"
#include "url.h"

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
		perror("collection.txt not found");
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

	if ((fp = fopen("collection.txt", "r")) == NULL) {
		perror("collection.txt not found");
		return;
	}
	while (i < url_count) {
		fscanf(fp, "%s", url);
		list[i] = malloc(100);
		strcpy(list[i], url);
		i++;
	}
	fclose(fp);
}

void getOutCount(int url_count, int *out_count) {
	FILE *fp;
	if ((fp = fopen("collection.txt", "r")) == NULL) {
		perror("collection.txt not found");
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
		fclose(fp2);
	}
	for (i = 0; i < url_count; i++) {
		out_count[i] -= 2;    //delete first 2 letters (#start Section-1)
	}
	fclose(fp);
}

int buildInWeights(double **Weights, int url_count, Graph G) {
	int i = 0;
	while (i < url_count) {
		int j = 0;
		while (j < url_count) {
			if (edgeExists(G, i, j)) {
				Weights[i][j] = getWeightIn(i, j, url_count, G);
			}
			j++;
		}
		i++;
	}
	return 1;
}

int buildOutWeights(double **Weights, int url_count, Graph G) {
	int i = 0;
	while (i < url_count) {
		int j = 0;
		while (j < url_count) {
			if (edgeExists(G, i, j) == 1) {
				Weights[i][j] = getWeightOut(i, j, url_count, G);
			}
			j++;
		}
		i++;
	}
	return 1;
}

double getWeightIn(int i, int j, int url_count, Graph G) {
	double weight = 0;

	double nom = getInLink(j, url_count, G);
	int x = 0;
	int denom = 0;
	while (x < url_count) {
		if (edgeExists(G, i, x)) {
			denom += getInLink(x, url_count, G);
		}
		x++;
	}

	weight = nom / denom;
	//printf("weight = %lf\n", weight);
	return weight;
}

double getWeightOut(int i, int j, int url_count, Graph G) {
	double weight = 0;

	double nom = getOutLink(j, url_count, G);
	int x = 0;
	int denom = 0;
	while (x < url_count) {
		if (edgeExists(G, i, x) == 1) {
			denom += getOutLink(x, url_count, G);
		}
		x++;
	}

	weight = nom / denom;
	//printf("weight = %lf\n", weight);
	return weight;
}

double getInLink(int i, int url_count, Graph G) {
	int x = 0;
	double count = 0;
	while (x < url_count) {
		if (edgeExists(G, x, i) == 1) count++;
		x++;
	}
	return count;
}

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

int getOutLinkID(int x, int url_count, Graph G, int *outLinkID) {
	int j = 0;
	for (int i = 0; i < url_count; i++) {
		if (edgeExists(G, x, i)) {
			outLinkID[j] = i;
			j++;
		}
	}
	return 1;
}
/*
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

	double **In_Weights = calloc(url_count, sizeof(double *));
	for (int i = 0; i < url_count; i++) {
		In_Weights[i] = calloc(url_count, sizeof(double));
	}
	assert(In_Weights != NULL);
	buildInWeights(In_Weights, url_count, g);

	double **Out_Weights = calloc(url_count, sizeof(double *));
	for (int i = 0; i < url_count; i++) {
		Out_Weights[i] = calloc(url_count, sizeof(double));
	}
	assert(Out_Weights != NULL);
	buildOutWeights(Out_Weights, url_count, g);
/*
    /////////// This is just printing out the Graph///////////////
    for (int i = 0; i < url_count; i++) {
        for (int j = 0; j < url_count; j++) {
            printf("%3lf ", In_Weights[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < url_count; i++) {
        for (int j = 0; j < url_count; j++) {
            printf("%3lf ", Out_Weights[i][j]);
        }
        printf("\n");
    }
    //showGraph2(g);
    /////////// This is just printing out the Graph///////////////


	double pr[url_count];    // array containing page ranks of each url
	double prOld[url_count];    // old page ranks

	// get initial pagerank for each URLs
	for (int i = 0; i < url_count; i++) {
		pr[i] = (double) 1 / url_count;
		prOld[i] = pr[i];
	}

	// get list of URLs
	char *url_list[url_count];
	getURLList(url_count, url_list);

	// get out degrees of URLs
	int outCount[url_count];
	for (int i = 0; i < url_count; i++) {
		outCount[i] = 0;
	}
	getOutCount(url_count, outCount);

	// pagerank algorithm
	int it = 0;
	double diff = diffPR;
	int *outID = calloc(url_count, sizeof(int));
	for(int i=0;i<url_count;i++){
		outID[i] = -1;
	}
	int sumPr = 0;
	while (it < maxIt && diff >= diffPR) {
		for (int i = 0; i < url_count; i++) {
			getOutLinkID(i, url_count, g, outID);
			for(int j=0;j<url_count;j++){
				sumPr += prOld[outID[j]];
			}
			pr[i] = ((1 - d) / url_count) + d*sumPr*In_Weights[i][j]*Out_Weights[i][j];
			double tmp = fabs(pr[i] - prOld[i]);
			diff += tmp;
			for (int k=0;k<url_count;k++){
				prOld[k] = pr[k];
			}
		}
		it++;
	}

	FILE *fp = fopen("pagerankList.txt", "w");
	for (int i = 0; i < url_count; i++) {
		fprintf(fp, "%s, %d, %.8lf\n", url_list[i], outCount[i], pr[i]);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
*/
