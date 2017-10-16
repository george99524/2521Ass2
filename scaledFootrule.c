// COMP2521: Assignment 2: Simple Search Engines
// Part-3: Hybrid/Meta Search Engine using Rank Aggregation
// Coded by George Bai/Eu Shaun Lim

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define BUFSIZE 100
#define MAX_URLS 100

int inList(char **list, char *url, int list_size);
void showList(char **list, int list_size);
double getScaledFootrule(char *url, int n_lists, char **list_names, int position, int total_positions);


int main(int argc, char *argv[]) {
	int n_lists = argc - 1;

	int *n_elements = calloc(n_lists, sizeof(int));
	char **url_list = malloc(MAX_URLS);
	//char url_list[MAX_URLS][BUFSIZE];
	//url_list[0] = NULL;
	char buffer[BUFSIZE];
	int n_urls = 0;
	for (int i = 0; i<n_lists; i++) {
		FILE *fp = fopen(argv[i+1], "r");
		while (fscanf(fp, "%s", buffer)!=EOF) {
			if (i==0) {
				url_list[n_urls] = malloc(BUFSIZE);
				strcpy(url_list[n_urls], buffer);
				n_urls++;
			}else
			if (inList(url_list, buffer, n_urls)==0) {
				url_list[n_urls] = malloc(BUFSIZE);
				strcpy(url_list[n_urls], buffer);
				n_urls++;
			}
			n_elements[i]++;
		}
		//printf("list %d has %d elements\n", i, n_elements[i]);
		fclose(fp);
	}
	//showList(url_list, n_urls);
	double matrix[n_urls][n_urls]; // creating a matrix of all possible scaled-footrule distances
	memset(matrix, 0.0, sizeof(double));
	//printf("nurls = %d\n", n_urls);
	for (int i = 0; i<n_urls; i++) {				// in urls x position form
		for (int pos = 1; pos<=n_urls; pos++) {
			//printf("pos=%d\n", pos);
			matrix[i][pos-1] = getScaledFootrule(url_list[i], n_lists, argv, pos, n_urls);
			//printf("1\n");
		}
		//printf("i=%d\n", i);
	}

	char row_flag[n_urls], column_flag[n_urls];
	memset(row_flag, 0.0, sizeof(char)*n_urls);
	memset(column_flag, 0.0, sizeof(char)*n_urls);
	int order[n_urls];
	int row, column;
	double total = 0.0, highest = -1.0;
	for (int i = 0; i<n_urls; i++) {
		for (int j = 0; j<n_urls; j++) {
			for (int k = 0; k<n_urls; k++) {
				if ((highest==-1.0 || matrix[i][j]<highest) && row_flag[i]==0 && column_flag[j]==0) {
					highest = matrix[i][j];
					row = i;
					column = j;
					order[i] = j;
				}
			}
		}
		total += highest;
		row_flag[row] = 1;
		column_flag[column] = 1;
		highest = -1.0;
	}

	printf("%lf\n", total);
	for (int i = 0; i<n_urls; i++) {
		for (int j = 0; j<n_urls; j++) {
			if (order[j]==i) printf("%s\n", url_list[j]);
		}
	}
	return 0;
}

int inList(char **list, char *url, int list_size) {
	if (list[0]==NULL) return 0;
	for (int i = 0; i<list_size; i++) {
		if (strcmp(list[i], url)==0) return 1;
	}
	return 0;
}

void showList(char **list, int list_size) {
	for (int i = 0; i<list_size; i++) {
		printf("%s\n", list[i]);
	}
}

double getScaledFootrule(char *url, int n_lists, char **list_names, int position, int total_positions) {
	double ScaledFootrule = 0.0;
	for (int i = 1; i<=n_lists; i++) {
		//printf("opening %s\n", list_names[i]);
		FILE *fp = fopen(list_names[i], "r");
		int list_position = 0, total_list_positions = 0;
		char buffer[BUFSIZE];
		while (fscanf(fp, "%s", buffer)!=EOF) {
			total_list_positions++;
			if (strcmp(buffer, url)==0) list_position = total_list_positions;
		}
		//printf("listpos = %d totalistpos = %d\n", list_position, total_list_positions);
		if (list_position!=0) ScaledFootrule += fabs((double) list_position/total_list_positions - (double) position/total_positions);
		list_position = total_list_positions = 0;
		//printf("ScaledFootrule = %lf for list %d\n", ScaledFootrule, i);
		fclose(fp);
	}
	return ScaledFootrule;
}
