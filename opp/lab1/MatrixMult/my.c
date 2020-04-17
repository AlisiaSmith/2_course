#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#include "fulling.h"

struct timeval tv1,tv2;

#define N 8
#define EPS 0.00000001
#define TAU 0.01

void show(double* vect, size_t size)
{
	for (int i = 0; i < size; ++i)
		printf("x[%d] = %10.8f \n", i, vect[i]);
}

void VECTxSCAL(double* res, double* vect, double scal, size_t size)
{
	for (int i = 0; i < size; ++i)
		res[i] = vect[i] * scal;
}

void MATxVECT(double* res, double* mat, double* vect, size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		double tmp = 0;
		for (int j = 0; j < size; ++j)
			tmp += mat[i * size + j] * vect[j];
		res[i] = tmp;
	}
}

void VECTsubVECT(double* res, double* vect1, double* vect2, size_t size)
{
	for (int i = 0; i < size; ++i)
		res[i] = vect1[i] - vect2[i];
}

void approx(double* res, double* xn, double* b, double* A, size_t size)
{
	double tau = TAU;

	MATxVECT(res, A, xn, size);
	VECTsubVECT(res, res, b, size);
	VECTxSCAL(res, res, tau, size);
	VECTsubVECT(res, xn, res, size);
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
	gettimeofday(&tv1,NULL);

	size_t size = N;


	double* A = (double*)malloc(sizeof(double) * N * N);
	double* b = (double*)malloc(sizeof(double) * N);
	double* x = (double*)malloc(sizeof(double) * N);
	double* next_x = (double*)malloc(sizeof(double) * N);

	full_1(A, b, x, size);

	double E = condition(x, b, A, size);
	while (E >= EPS)
	{

		approx(next_x, x, b, A, size);

		double* tmp = next_x;
		next_x = x;
		x = tmp;
		E = condition(x, b, A, size);
	}

	gettimeofday(&tv2,NULL);


	double dt_sec = (tv2.tv_sec, tv1.tv_sec);
	double dt_usec = (tv2.tv_usec, tv1.tv_usec);
	double dt = dt_sec + 1e-6*dt_usec;
	printf("time diff %e \n",dt);

	free(next_x);
	free(x);
	free(b);
	free(A);

	return 0;
}
