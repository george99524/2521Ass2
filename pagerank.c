#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include <unistd.h>

Graph buildGraph();
int getID(char* string, char** list, int url_count);
int getURLCount();
void getURLList(int url_count, char** list);


// Builds Weighted Graph from collections.txt
Graph buildGraph() {

    int url_count = getURLCount();

    Graph G = newGraph(url_count);
    char *url_list[url_count];
    getURLList(url_count, url_list);

    FILE *fp;
    fp = fopen("collection.txt", "r");
    char buffer[100], url[100];
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
                    continue;
                }
            }
            if (strcmp(url, end)==0 && flag==1) {
                break;
            }

            if (flag==1) {
                //printf("%s\n", url);
                //printf("%d\n", getID(url, url_list, url_count));
                insertEdge(G, getID(buffer, url_list, url_count), getID(url, url_list, url_count), 1);
            }
        }
        //printf("\n");
    }

    fclose(fp);
    showGraph2(G);
    return G;
}

int main(void) {

    buildGraph();
    return 0;
}

int getID(char* string, char** list, int url_count) {
    int id = 0;
    while(id<url_count && strcmp(list[id], string)) {
        id++;
    }
    return id == url_count ? -1 : id;
}

int getURLCount() {
    FILE *fp;
    if ((fp = fopen("collection.txt", "r"))==NULL) {
        perror("No such file");
        return 0;
    }

    char url[100];
    int url_count = 0;
    while (fscanf(fp, "%s", url)!=EOF) {
        url_count++;
        //printf("%s\n", url);
    }
    fclose(fp);

    return url_count;
}

void getURLList(int url_count, char** list) {
    int i = 0;
    FILE *fp;
    char url[100];

    fp = fopen("collection.txt", "r");
    while (i<url_count) {
        fscanf(fp, "%s", url);
        list[i] = malloc(100);
        strcpy(list[i], url);
        i++;
    }
}


