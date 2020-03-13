#include <stdlib.h>

#include "fulling.h"



/*	double *A;
	if (thread_id==0) {
		double *Matrix= (double*) malloc(N*N*sizeof(double));
		for (int i=0;i<N;i++)
		for (int j=0;j<N;j++) {
			Matrix[i*N+j] = (i==j) ? 2 : 1;
		}

		//отдать каждому процессу его строки
		for(int i=1;i<size_proc;i++)
		{
			MPI_Send(Matrix+i*rows*N, rows*N, MPI_DOUBLE, i, 123, MPI_COMM_WORLD);
	    }
	    A = Matrix;

    } else {
    	A = (double*) malloc(rows*N*sizeof(double));
    	MPI_Recv(A, rows*N, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	//получить отданные строки в буфер по номеру процесса
    }*/





void full_1(double* A, double* b, double* x, size_t size, int n, int np, int pid)
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < n; ++j)
			A[i*size + j] = (pid * size + i == j) ? 2.0 : 1.0;

	for (int i = 0; i < n; ++i)
	{
		b[i] = size + 1;
		x[i] = 0;
	}
}

void full_2(double* A, double* b, double* x, size_t size, int n, int np, int pid)
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			A[i*size + j] = (i == j) ? 2.0 : 1.0;

	for (int i = 0; i < size; ++i)
		x[i] = size - i;
	
	MATxVECT(b, A, x, size);
	
	for (int i = 0; i < size; ++i)
		x[i] = 0;
}
