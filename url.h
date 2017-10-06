// pagerank functions

#ifndef PAGERANK_H
#define PAGERANK_H

#include "graph.h"

Graph buildGraph();

int getID(char *string, char **list, int url_count);

int getURLCount();

void getURLList(int url_count, char **list);

void getOutCount(int url_count, int *out_count);

void PageRankW(Graph g, double d, double diffPR, double maxIt, int url_count);

int buildInWeights(double **Weights, int url_count, Graph G);

int buildOutWeights(double **Weights, int url_count, Graph G);

double getWeightIn(int i, int j, int url_count, Graph G);

double getWeightOut(int i, int j, int url_count, Graph G);

double getInLink(int i, int url_count, Graph G);

double getOutLink(int i, int url_count, Graph G);

#endif
