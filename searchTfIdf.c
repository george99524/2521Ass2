// COMP2521: Assignment 2: Simple Search Engines
// Part-2: Content-based Search Engine
// Coded by George Bai/Eu Shaun Lim

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "graph.h"
#include <unistd.h>
#include "url.h"


#define BUFSIZE 100

int read_urls();
double get_tf(char *word, char *url);
int isURL(char *word, int n_urls);
int getIndex(char *url, int n_urls);
void normalise(char word[]);
void print_results(double *total, int n_urls);

char **urls;

int main(int argc, char *argv[]) {
	int n_urls = read_urls();
	int n_words = argc - 1;
	FILE *fp;
	char url[BUFSIZE], buffer[BUFSIZE];

	double tf_idf[n_words][n_urls];
	memset(tf_idf, 0.0, sizeof(double) * n_words * n_urls);

	double tf[n_words][n_urls];
	memset(tf, 0.0, sizeof(double) * n_words * n_urls);

	double total[n_urls];
	memset(total, 0.0, sizeof(double) * n_urls);

	int *url_count = calloc(n_words, sizeof(int));
	for (int i=1; i<=n_words; i++) {
		fp = fopen("invertedIndex.txt", "r");
		while (fscanf(fp, "%s", url)!=EOF) {
			if (strcmp(url, argv[i])==0) {
				int j = 0;
				int total_pages = 0;
				while (fscanf(fp, "%s", buffer)!=EOF && isURL(buffer, n_urls)==1) {
					total_pages++;
					tf[i-1][getIndex(buffer, n_urls)] = get_tf(argv[i], buffer);
					url_count[i-1]++;
					//printf("url count for word %s is %d\n", argv[i], url_count[i-1]);
					//printf("%s\n", argv[i]);
					//printf("%s\n", buffer);
					//printf("%d\n", getIndex(buffer, n_urls));
				}
				break;
			}
		}
	}
	for (int i=0; i<n_words; i++) {
		for (int j=0; j<n_urls; j++) {
			//printf("tf = %lf\n", tf[i][j]);
			//printf("n_w = %d\n", url_count[i]);
			tf_idf[i][j] = tf[i][j] * log10((double) n_urls/url_count[i]);
			if (url_count[i]==0 || tf[i][j]==0.0) tf_idf[i][j] = 0.0;
			//printf("tf_idf = %lf\n\n", tf_idf[i][j]);

		}
		//printf("\n");
	}
	for (int i=0; i<n_urls; i++) {
		for (int j=0; j<n_words; j++) {
			total[i] += tf_idf[i][j];
		}
		//printf("total = %lf\n", total[i]);
	}
	print_results(total, n_urls);
	return 0;
}

int read_urls() {
	char buffer[BUFSIZE];
	FILE *fp = fopen("collection.txt", "r");
	int n_urls = 0;;
	while (fscanf(fp, "%s", buffer)!=EOF) {
		n_urls++;
	}
	fclose(fp);
	//printf("%d\n", n_urls);
	urls = malloc(sizeof(char*)*n_urls);
	int i = 0;
	fp = fopen("collection.txt", "r");
	while (fscanf(fp, "%s", buffer)!=EOF && i<n_urls) {
		urls[i] = malloc(sizeof(buffer));
		strcpy(urls[i], buffer);
		//printf("read %s\n", urls[i]);
		i++;
	}
	//printf("1st url %s\n", urls[0]);
	fclose(fp);

	return n_urls;
}

double get_tf(char *word, char *url) {
	char filename[BUFSIZE];
	//printf("%s\n", url);
	strcpy(filename, url);
	strcat(filename, ".txt");
	FILE *fp = fopen(filename, "r");
	char buffer[BUFSIZE];
	int word_count = 0, total_count = 0;
	while (fscanf(fp, "%s", buffer)!=EOF) {
		char start[] = "#start\0";
		char end[] = "#end\0";
		char section2[] = "Section-2\0";
		if (strcmp(buffer, start)==0) {
			fscanf(fp, "%s", buffer);
			if (strcmp(buffer, section2)==0) {
				while (fscanf(fp, "%s", buffer)!=EOF) {
					if (strcmp(buffer, end)==0) break;
					normalise(buffer);
					//printf("%s ", buffer);
					if (strcmp(buffer, word)==0) word_count++;
					total_count++;
				}
			}
		}
		//printf("%s %s\n", start, end);
	}
	//printf("\n%d %d\n", word_count, total_count);
	return (double) word_count/total_count;
}


int isURL(char *word, int n_urls) {
	int i = 0;
	while (i<n_urls) {
		if (strcmp(urls[i], word)==0) return 1;
		i++;
	}
	return 0;
}

int getIndex(char *url, int n_urls) {
	int  i = 0;
	while (i<n_urls) {
		if (strcmp(urls[i], url)==0) return i;
		i++;
	}
	return -1;
}

void normalise(char word[]) {

	// convert to lower case
	//for (int i = 0; word[i]; i++) {
	//	word[i] = tolower(word[i]);
	//}
	// remove all non-alphabets
	for (int i = 0; word[i]; i++) {
		word[i] = tolower(word[i]);
		while (!(isalpha(word[i]) || word[i] == '\0')) {
			int j = 0;
			for (j = i; word[j] != '\0'; ++j) {
				word[j] = word[j + 1];
			}
			word[j] = '\0';
		}
	}
}

void print_results(double *total, int n_urls) {
	char flag[n_urls];
	int i = 0;
	while (i<n_urls && i<30) {
		double greatest = 0.0;
		int index;
		for (int j = 0; j<n_urls; j++) {
			if (total[j]>greatest && flag[j]!=1) {
				greatest = total[j];
				index = j;
			}
		}
		printf("%s %.6f\n", urls[index], greatest);
		flag[index] = 1;
		i++;
	}
}
