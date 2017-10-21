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
#include "function.h"


#define BUFSIZE 100

int read_urls();
double get_tf(char *word, char *url);
int isURL(char *word, int n_urls);
int getIndex(char *url, int n_urls);
void print_results(double *total, int n_urls, int *word_appearance);
int get_word_appearance(char *word, char *url);

char **urls;

int main(int argc, char *argv[]) {
	int n_urls = read_urls();
	int n_words = argc - 1;
	char *words[n_words];
	for (int i = 0; i < n_words; i++) {
		words[i] = malloc(sizeof(argv[i+1]));
		strcpy(words[i], argv[i+1]);
		normalise(words[i]);
		//printf("%s\n", words[i]);
	}

	FILE *fp;
	char url[BUFSIZE], buffer[BUFSIZE];

	double tf_idf[n_words][n_urls];
	memset(tf_idf, 0.0, sizeof(double) * n_words * n_urls);

	double tf[n_words][n_urls];
	memset(tf, 0.0, sizeof(double) * n_words * n_urls);

	double total[n_urls];
	memset(total, 0.0, sizeof(double) * n_urls);

	int *url_count = calloc(n_words, sizeof(int));
	for (int i = 0; i < n_words; i++) {
		fp = fopen("invertedIndex.txt", "r");
		while (fscanf(fp, "%s", url)!=EOF) {
			if (strcmp(url, words[i])==0) {
				int total_pages = 0;
				while (fscanf(fp, "%s", buffer)!=EOF && isURL(buffer, n_urls)==1) {
					total_pages++;
					tf[i][getIndex(buffer, n_urls)] = get_tf(words[i], buffer);
					url_count[i]++;
					//printf("url count for word %s is %d\n", words[i], url_count[i-1]);
					//printf("%s\n", words[i]);
					//printf("%s\n", buffer);
					//printf("%d\n", getIndex(buffer, n_urls));
				}
				break;
			}
		}
	}
	for (int i=0; i<n_words; i++) {
		for (int j=0; j<n_urls; j++) {
			//printf("for url %s\n", urls[j]);
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
			total[i] += tf_idf[j][i];
		}
		//printf("total = %lf\n", total[i]);
	}
	int *word_appearance = calloc(n_urls, sizeof(int));
	for (int i=0; i<n_words; i++) {
		for (int j=0; j<n_urls; j++) {
			word_appearance[j] += get_word_appearance(words[i], urls[j]);
		}
	}

	print_results(total, n_urls, word_appearance);
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

void print_results(double *total, int n_urls, int *word_appearance) {
	char *flag = calloc(n_urls, sizeof(char));
	int i = 0;
	while (i<n_urls && i<30) {
		double greatest = 0.0;
		int greatest_word_appearance = 0;
		int index;
		for (int j = 0; j<n_urls; j++) {
			if (word_appearance[j]>greatest_word_appearance && flag[j]!=1) {
				greatest = total[j];
				index = j;
				greatest_word_appearance = word_appearance[j];
			}else
			if (word_appearance[j]>=greatest_word_appearance && total[j]>greatest && flag[j]!=1) {
				greatest = total[j];
				index = j;
				greatest_word_appearance = word_appearance[j];
			}
		}
		if (greatest!=0) printf("%s %.6f with %d word appearances\n", urls[index], greatest, greatest_word_appearance);
		flag[index] = 1;
		i++;
	}
}

int get_word_appearance(char *word, char *url) {

	char filename[BUFSIZE], buffer[BUFSIZE];
	strcpy(filename, url);
	strcat(filename, ".txt");
	FILE *fp = fopen(filename, "r");
	while (fscanf(fp, "%s", buffer)!=EOF) {
		normalise(buffer);
		if (strcmp(buffer, word)==0) return 1;
	}
	return 0;
}
