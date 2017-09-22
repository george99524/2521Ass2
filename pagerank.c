#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include <unistd.h>

Graph buildGraph();

// Builds Weighted Graph from collections.txt
Graph buildGraph() {

    FILE *fp;
    if ((fp = fopen("collection.txt", "r"))==NULL) {
        perror("No such file");
        return NULL;
    }

    char url[10];
    int url_count = 0;
    while (fscanf(fp, "%s", url)!=EOF) {
        url_count++;
        //printf("%s\n", url);
    }
    fclose(fp);
    printf("%d\n", url_count);
    Graph G = newGraph(url_count);

    fp = fopen("collection.txt", "r");
    char buffer[100];
    while (fscanf(fp, "%s", buffer)!=EOF) {
        strcat(buffer, ".txt");
        //printf("%s\n", buffer);

    }

    fclose(fp);
    return G;
}

int main(void) {

    buildGraph();
    return 0;
}
