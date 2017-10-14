#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include <unistd.h>
#include "math.h"
#include "url.h"

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
		fprintf(fp, "%s, %d, %.7lf\n", url_list[i], outCount[i], pr[i]);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}

