#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
//#include <mpi.h>


#define N 2
#define Eps  0.00001

void mulScalar(double *v, double *v1, double scalar, int size) {

	for (int j = 0; j < size; ++j) {
		v[j] = v1[j] * scalar;
	}
}

void mulVect(double *v1, double *v2, double *v, int size) {

	for (int j = 0; j < size; ++j) {
		v[j] = v1[j] * v2[j];
	}
}

void minusVect(double *v1, double *v2, double *v, int size) {

	for (int j = 0; j < size; ++j) {
		v[j] = v1[j] - v2[j];
	}
}

double ScalarMul(double *v1, double *v2, int size) {

	double *v = (double*) malloc(size * sizeof(double));
	double rez = 0;

	mulVect(v1, v2, v, size);

	for (int j = 0; j < size; ++j) {
		rez = rez + v[j];
	}

	free(v);
	return rez;
}


void mulMatrix(double *A, double *v, double *rez, int numCell) {

	double *tmp = (double*)malloc(N * sizeof(double));
	int columns, i = 0;

	while (columns != numCell / N) {
		mulVect(&(A[columns*N]), v, tmp, N);
		rez[i] = 0;
		for (int j = 0; j < N; ++j) {
			rez[i] = rez[i] + tmp[j];
		}
		columns++;
	}
	free(tmp);
}

void Print(double *v, int size, double *A, double * b, double * y, double * t, double * tmp) {
	for (int i = 0; i < size; i++)
	{
		printf("%f ", v[i]);
	}
	printf("\n");
	free(v);
	free(A);
	free(b);
	free(y);
	free(t);
	free(tmp);
}


double norma(double *v, int size) {

	double rez = 0;
	double *t = (double*)malloc(size * sizeof(double));

	for (int j = 0; j < size; ++j) {
		t[j] = v[j] * v[j];
		rez = rez + t[j];
	}
	free(t);
	return sqrt(rez);
}

int main(int argc, char **argv) {
	int size = N;
	float E = Eps;
	double scal = 0;
	int index = 0;
	int rows = 0;
	int mod = 0;
	double *A = (double*)malloc(size *size * sizeof(double));
	double *buf = (double*)malloc(size * sizeof(double));
	double *x = (double*)malloc(size * sizeof(double));
	double *b = (double*)malloc(size * sizeof(double));
	double *y = (double*)malloc(size * sizeof(double));
	double *t = (double*)malloc(size * sizeof(double));
	double *tmp = (double*)malloc(size * sizeof(double));
	//MPI_Init(&argc, &argv);
	//int rank, numcell, size_proc = 0;
	//MPI_Status st;
	//MPI_Comm_size(MPI_COMM_WORLD, &size_proc); //????? ?????????
	//MPI_Comm_rank(MPI_COMM_WORLD, &rank);      //????? ????????
	// ???????? ???? ?????????
	//MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//??? ?????????? ??????? ?? ??????, ??????? ?????????? ?????? ??? ?????
	int numCells = N * N;
	mod = (N*N) /*% size_proc*/;
	rows = (N*N) /*/ size_proc*/;

	//if (rank == 0) {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (i == j) A[i * size + j] = 2;
				else A[i * size + j] = 1;
			}
		}
	//}
	//else {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				A[i * size + j] = 0;
			}
		}
	//}

	for (int j = 0; j < size; ++j) {
		x[j] = 0;
		tmp[j] = 0;
	}
	for (int j = 0; j < size; ++j) {
		b[j] = N + 1;
	}
	//???????? ??????????? ?????? ???????
	//if (rank == 0) {
		for (int i = 1; i < N; i++) {
			if (rank == 0 && mod != 0) {
				MPI_Send(A, N*(rows + 1), MPI_DOUBLE, i, 123, MPI_COMM_WORLD);
				numcell = N * (rows + 1);
				MPI_Send(&numcell, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
				mod--;
				A = (A + N * (rows + 1));
			}
			if (rank == 0 && mod == 0)
			{
				MPI_Send(A, N*rows, MPI_DOUBLE, i, 123, MPI_COMM_WORLD);
				numcell = N * (rows);
				MPI_Send(&numcell, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
				A = (A + N * rows);
			}
		}
	}
	else {
		MPI_Recv(A, N*N, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(numcell, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	//???????? ????
	mulMatrix(A, x, y, size); //Ax
	minusVect(y, b, y, size); //Ax-b
	while ((norma(y, size) / norma(b, size)) >= E)
	{
		mulMatrix(A, y, t, size);
		scal = ScalarMul(t, y, size) / ScalarMul(t, t, size);//tau
		mulScalar(tmp, y, scal, size);
		minusVect(x, tmp, x, size);
		mulMatrix(A, x, y, numcell); //Ax
		minusVect(y, b, y, size); //Ax-b
	}
	MPI_Reduce(x, x, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		Print(x, size, A, b, y, t, tmp);
	return 0;
}