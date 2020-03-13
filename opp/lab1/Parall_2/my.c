#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>

#include "fulling.h"

struct timeval tv1,tv2;

#define N 12
#define EPS 0.0000001
#define TAU 0.001


void show(double* vect, size_t size)
{
	for (int i = 0; i < size; ++i)
		printf("x[%d] = %f \n", i, vect[i]);
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

void approx(double* res, double* xn, double* b, double* A, size_t size, int n)
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
	int size_proc, thread_id;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);    

	if (N % size_proc!=0) {
		if (thread_id == 0)
			printf("Invalid parameters\n");
		MPI_Finalize();
		return 0;
	}

	
	gettimeofday(&tv1,NULL);

	size_t size = N / size_proc;
	
	double* A = (double*)malloc(sizeof(double) * N * size);
	double* b = (double*)malloc(sizeof(double) * N);
	double* xbuf = (double*)malloc(sizeof(double) * N);
	double* next_xbuf = (double*)malloc(sizeof(double) * N);
	double* x;	
	double* Matrix;	

	double E;

	full_1(A, b, xbuf, size);

	if (thread_id == 0) 
	{
		x = (double*)malloc(sizeof(double) * N);
		for (int i = 0; i < N; i++)
			x[i] = 0;
		Matrix = (double*)malloc(sizeof(double) * N * N);
		
		for (int i=0;i<N;i++)
			for (int j=0;j<N;j++) {
				Matrix[i*N+j] = (i==j) ? 2 : 1;
		
		E = condition(x, b, Matrix, N);
		for(int i = 1; i < size_rpoc; i++)
			MPI_Send(&E, 1, MPI_DOUBLE, i, 123, MPI_COMM_WORLD);
	}	else	{
	
		MPI_Recv(&E, 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	}

	
	 

	while (E >= EPS )
	{
		approx(next_xbuf, xbuf, b, A, size);
		
		double* tmp = next_xbuf;
		next_x = xbuf;
		xbuf = tmp;
		
		if(thread_id == 0) 
		{
			for(int i = 1; i < size_rpoc; i++)
				MPI_Recv(x + size*i, i, MPI_DOUBLE, i, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			E = condition(x, b, Matrix, N);
			for(int i = 1; i < size_rpoc; i++)
				MPI_Send(&E, 1, MPI_DOUBLE, i, 123, MPI_COMM_WORLD);
		}	else		{
			MPI_Send(xbuf, size, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD);
			MPI_Recv(&E, 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		}

	}
	
	gettimeofday(&tv2,NULL);
	//show(x, size);

	
	if(thread_id == 0) 
	{
		double dt_sec = (tv2.tv_sec, tv1.tv_sec);
		double dt_usec = (tv2.tv_usec, tv1.tv_usec);
		double dt = dt_sec + 1e-6*dt_usec;
		printf("time diff %e \n",dt);
		free(x);
		free(Matrix);
	}
	free(next_xbuf);
	free(xbuf);
	free(b);
	free(A);

	MPI_Finalize();
	return 0;
}
