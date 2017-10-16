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
	int n_lists = argc - 1; // # rankings is how many args are supplied to the program

	int *n_elements = calloc(n_lists, sizeof(int)); // array storing the number of elements for each list (rankings)
	char **url_list = malloc(MAX_URLS); // array storing all unique strings in all lists
	//char url_list[MAX_URLS][BUFSIZE];
	//url_list[0] = NULL;
	char buffer[BUFSIZE];
	int n_urls = 0; // number of urls across all lists
	for (int i = 0; i<n_lists; i++) {
		FILE *fp = fopen(argv[i+1], "r"); // open each list
		while (fscanf(fp, "%s", buffer)!=EOF) { // scan each url in list
			if (i==0) { // if this is the first list scanned, add every url in the list
				url_list[n_urls] = malloc(BUFSIZE);
				strcpy(url_list[n_urls], buffer);
				n_urls++;
			}else      // else, check if the url is already in list, add if not in list
			if (inList(url_list, buffer, n_urls)==0) {
				url_list[n_urls] = malloc(BUFSIZE);
				strcpy(url_list[n_urls], buffer);
				n_urls++;
			}
			n_elements[i]++; // count number of elements in each list
		}
		//printf("list %d has %d elements\n", i, n_elements[i]);
		fclose(fp);
	}
	//showList(url_list, n_urls);
	double matrix[n_urls][n_urls]; // creating a matrix of all possible scaled-footrule distances
	memset(matrix, 0.0, sizeof(double)); // in (urls x position) form
	//printf("nurls = %d\n", n_urls);
	for (int i = 0; i<n_urls; i++) {
		for (int pos = 1; pos<=n_urls; pos++) { // compute scaled-footrule distance for every
			//printf("pos=%d\n", pos);          // possible combination for url and position
			matrix[i][pos-1] = getScaledFootrule(url_list[i], n_lists, argv, pos, n_urls);
			//printf("1\n");
		}
		//printf("i=%d\n", i);
	}

	char row_flag[n_urls], column_flag[n_urls]; // boolean flags for each row and column in the matrix
	memset(row_flag, 0.0, sizeof(char)*n_urls);
	memset(column_flag, 0.0, sizeof(char)*n_urls);
	int order[n_urls]; // array storing the order(ranking) of the urls, the index is the url, the value is the rank
	int row, column; // values to store the column/row value of the lowest value
	double total = 0.0, lowest = -1.0; // value for total scaled-footrule distance, lowest distance in the matrix
	for (int i = 0; i<n_urls; i++) { // compute the order number for each url
		for (int j = 0; j<n_urls; j++) { // scanning the matrix for the lowest value
			for (int k = 0; k<n_urls; k++) {
				if ((lowest==-1.0 || matrix[i][j]<lowest) && row_flag[i]==0 && column_flag[j]==0) {
					lowest = matrix[i][j];
					row = i;     // store row/column of lowest value
					column = j;
					order[i] = j; // store the order value for the url
				}
			}
		}
		total += lowest; // add lowest value to total
		row_flag[row] = 1; // mark the row and column to never be considered after
		column_flag[column] = 1; // this is the same as marking the position/url to never be used again
		lowest = -1.0; // reset lowest value
	}

	printf("%lf\n", total); // printing the results
	for (int i = 0; i<n_urls; i++) {
		for (int j = 0; j<n_urls; j++) {
			if (order[j]==i) printf("%s\n", url_list[j]);
		}
	}
	return 0;
}

// Function to check whether a url is currently in the list
int inList(char **list, char *url, int list_size) {
	if (list[0]==NULL) return 0;
	for (int i = 0; i<list_size; i++) {
		if (strcmp(list[i], url)==0) return 1;
	}
	return 0;
}

// Function to show the supplied list
void showList(char **list, int list_size) {
	for (int i = 0; i<list_size; i++) {
		printf("%s\n", list[i]);
	}
}

// Function to compute the total scaled-footrule distance for a specific final position for a url
double getScaledFootrule(char *url, int n_lists, char **list_names, int position, int total_positions) {
	double ScaledFootrule = 0.0;
	for (int i = 1; i<=n_lists; i++) { // compute the distance for each ranking supplied
		//printf("opening %s\n", list_names[i]);
		FILE *fp = fopen(list_names[i], "r"); // open each ranking
		int list_position = 0, total_list_positions = 0; // marks for the position of the url in each ranking
		char buffer[BUFSIZE];
		while (fscanf(fp, "%s", buffer)!=EOF) {
			total_list_positions++;
			if (strcmp(buffer, url)==0) list_position = total_list_positions;
		}
		//printf("listpos = %d totalistpos = %d\n", list_position, total_list_positions);
		// if the url is in the ranking, add the distance to the total
		if (list_position!=0) ScaledFootrule += fabs((double) list_position/total_list_positions - (double) position/total_positions);
		list_position = total_list_positions = 0;
		//printf("ScaledFootrule = %lf for list %d\n", ScaledFootrule, i);
		fclose(fp);
	}
	return ScaledFootrule;
}
