#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void numerical(int m, double ** Q, double ** bounds, double *mean, double lambda, double *x);
//call the function of numerical method
int m, i, j, retcode = 0;
char mybuffer[100];
char * filename;
double ** bounds, ** Q, *mean, *x, lambda;
FILE *in = NULL;

int readfile(char * filename)//read file
{
	in = fopen(filename, "r");
	if (in == NULL) {
		printf("could not open %s for reading\n", filename);
		return 200;
	}

	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	m = atoi(mybuffer);//read the number of assets

	bounds = (double **)calloc(m, sizeof(double));//bounds of each assets
	for (i = 0; i <= m - 1; i++) {
		bounds[i] = (double *)calloc(2, sizeof(double));
	}
	if (bounds == NULL) {
		printf("no memory\n"); return 400;
	}

	Q = (double **)calloc(m, sizeof(double));//covariance metrix
	for (i = 0; i <= m - 1; i++) {
		Q[i] = (double *)calloc(m, sizeof(double));
	}
	if (Q == NULL) {
		printf("no memory\n"); return 400;
	}

	mean = (double *)calloc(m, sizeof(double));//mean of return
	if (mean == NULL) {
		printf("no memory\n"); return 400;
	}

	x = (double *)calloc(m, sizeof(double));//proportion of each asset
	if (x == NULL) {
		printf("no memory\n"); return 400;
	}

	fscanf(in, "%s", mybuffer);
	for (j = 0; j <= m - 1; j++) {
		fscanf(in, "%s", mybuffer);
		fscanf(in, "%s", mybuffer);
		bounds[j][0] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		bounds[j][1] = atof(mybuffer);
		fscanf(in, "%s", mybuffer);
		mean[j] = atof(mybuffer);

	}

	fscanf(in, "%s", mybuffer);
	fscanf(in, "%s", mybuffer);
	lambda = atof(mybuffer);

	fscanf(in, "%s", mybuffer);
	for (j = 0; j <= m - 1; j++) {
		for (i = 0; i <= m - 1; i++) {
			fscanf(in, "%s", mybuffer);
			Q[j][i] = atof(mybuffer);
		}
	}

	fscanf(in, "%s", mybuffer);
	if (strcmp(mybuffer, "END") != 0) {
		printf("possible error in data file: 'END' missing\n");
	}
	fclose(in);
	return 0;
}


int main(int argc, char * argv[])
{
	if (argc != 2) {
		printf("Usage: exe datafilename\n"); retcode = 100; goto BACK;
	}
	filename = argv[1];//read the data file
	retcode = readfile(filename);
	if (retcode)
		goto BACK;

/*
	for (j = 0; j <= m - 1; j++) {
		for (i = 0; i <= m - 1; i++) {
			printf("%f ", Q[j][i]);
		}
	}
*/
	for (j = 0; j <= m - 1; j++) {
		if (Q[j][j]<0) {
			printf("Variance of x should be larger than 0.\n"); retcode = 400; goto BACK;
		}
		else {
			for (i = 0; i<j; i++) {
				if (Q[j][i] != Q[i][j]) {
					printf("Matrix Q should be symmetric.\n"); retcode = 400; goto BACK;
				}
			}
		}
	}

	numerical(m, Q, bounds, mean, lambda, x);
	for (i = 0; i <= m - 1; i++) {
		free(Q[i]);
	}
	for (i = 0; i <= m - 1; i++) {
		free(bounds[i]);
	}
	free(bounds);
	free(Q);
	free(mean);
	free(x);

BACK:
	return retcode;
}
