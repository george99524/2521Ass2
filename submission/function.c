// COMP2521: Assignment 2: Simple Search Engines
// functions that need to be used by multiple files
// Coded by George Bai/Eu Shaun Lim

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "function.h"

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

void normalise(char word[]) {

	// convert to lower case and
	// remove all non-alphabets
	for (int i = 0; word[i]; i++) {
		word[i] = tolower(word[i]);
		while (!(isalnum(word[i]) || word[i] == '\0')) {
			int j = 0;
			for (j = i; word[j] != '\0'; j++) {
				word[j] = word[j + 1];
			}
			word[j] = '\0';
		}
	}
}

char *str_dup(char *old) {
	char *new;
	int len = strlen(old);

	new = malloc(len + 1);
	if (new) {
		memcpy(new, old, len + 1);
	}
	return new;
}

int isURL(char *word, int n_urls) {
	int i = 0;
	int n = getURLCount();
	char *urls[n];
	getURLList(n, urls);
	while (i<n_urls) {
		if (strcmp(urls[i], word)==0) return 1;
		i++;
	}
	return 0;
}