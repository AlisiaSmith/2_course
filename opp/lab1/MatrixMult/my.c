#include <stdlib.h>
#include <math.h>

#include "fulling.h"

#define N 32
#define EPS 0.00001
#define TAU -0.001


double* VECTxSCAL(double* res, double* vect, double scal, size_t size)
{
	for (int i = 0; i < size; ++i)
		res[i] = vect[i] * scal;
	return res;
}

double* MATxVECT(double* res, double* mat, double* vect, size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		double tmp = 0;
		for (int j = 0; j < size; ++j)
			tmp += mat[i * size + j] * vect[j];
		res[i] = tmp;
	}

		return res;
}

double* VECTsubVECT(double* res, double* vect1, double* vect2, size_t size)
{
	for (int i = 0; i < size; ++i)
		res[i] = vect1[i] - vect2[i];
	return res;
}

double* approx(double* res, double* xn, double* b, double* A, size_t size)
{
	double tau = TAU;

	MATxVECT(res, A, xn, size);
	VECTsubVECT(res, res, b, size);
	VECTxSCAL(res, res, tau, size);
	VECTsubVECT(res, xn, res, size);

	return res;
}

double norm(double* vect, size_t size)
{
	double res = 0;
	for (int i = 0; i < size; ++i)
		res += vect[i] * vect[i];
	return sqrt(res);
}

double condition(double* xn, double* b, double* A, size_t size)
{
	double* tmp = (double*)malloc(sizeof(double) * N);
	
	MATxVECT(tmp, A, xn, size);
	VECTsubVECT(tmp, tmp, b, size);

	double n = norm(tmp, size) / norm(b, size);
	
	free(tmp);
	
	return n;
}

int main(int argc, char **argv)
{
	size_t size = N;
	double E = EPS;

	double* A = (double*)malloc(sizeof(double) * N * N);
	double* b = (double*)malloc(sizeof(double) * N);
	double* x = (double*)malloc(sizeof(double) * N);
	double* next_x = (double*)malloc(sizeof(double) * N);

	full_1(A, b, x, size);

	while (condition(x, b, A, size) >= E)
	{
		approx(next_x, x, b, A, size);
		
		double* tmp = next_x;
		next_x = x;
		x = tmp;
	}


	free(next_x);
	free(x);
	free(b);
	free(A);

	return 0;
}