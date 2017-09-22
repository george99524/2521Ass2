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

    char url[100];
    int url_count = 0;
    while (fscanf(fp, "%s", url)!=EOF) {
        url_count++;
        //printf("%s\n", url);
    }
    fclose(fp);
    //printf("%d\n", url_count);

    Graph G = newGraph(url_count);

    fp = fopen("collection.txt", "r");
    char buffer[100];
    while (fscanf(fp, "%s", buffer)!=EOF) {
        char temp_string[1000];
        strcpy(temp_string, buffer);
        strcat(temp_string, ".txt");
        //printf("%s\n", buffer);

        FILE *temp = fopen(temp_string, "r");
        int flag = 0;
        while (fscanf(temp, "%s", url)!=EOF) {
            char section1[] = "Section-1";
            char end[] = "#end";

            if (strcmp(url, section1)==0) {
                if (flag==0) {
                    flag = 1;
                }
            }
            if (strcmp(url, end)==0 && flag==1) {
                break;
            }

            if (flag==1) {
                printf("%s\n", url);
                //WE NEED TO ADD EDGES BETWEEN THE VERTICES NOW

            }
        }
        printf("\n");
    }

    fclose(fp);
    return G;
}

int main(void) {

    buildGraph();
    return 0;
}
