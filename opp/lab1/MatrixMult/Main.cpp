#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
int N = 12000;
double Eps = 0.0001;

double ScalarMul(double* v1, double* v2, int sz) {
	double res = 0;
	for (int i = 0; i < sz; i++) {
		res += v1[i] * v2[i];
	}
}
void minusVect(double* v1, double* v2, double* res, int sz) {
	for (int i = 0; i < sz; i++) {
		res[i] = v1[i] - v2[i];
	}
}
void mulMatrix(double* A, double* x, double* res, int num_cells) {
	int rows = num_cells / N;
	for (int i = 0; i < rows; i++) {
		res[i] = ScalarMul(&A[N*i], x, N);
	}
}

double norma(double *v, int size) {

	double rez = 0;
	for (int j = 0; j < size; ++j) {
		rez = rez + v[j] * v[j];
	}
	return sqrt(rez);
}

void Print(double* v, int sz) {
	printf("Vect: ");
	for (int i = 0; i < sz; i++) {
		printf("%lf ", v[i]);
	}
	printf("\n");
}

void mulScalar(double* res, double* v, double consta, int sz) {
	for (int i = 0; i < sz; i++) {
		res[i] = v[i] * consta;
	}
}
int main(int argc, char **argv) {

	int size_proc, thread_id;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
	if (N % size_proc != 0) {
		if (thread_id == 0)
			printf("Invalid parameters\n");
		MPI_Finalize();
		return 0;
	}
	double time_begin = MPI_Wtime();
	int rows = N / size_proc;

	double *A;
	if (thread_id == 0) {
		double *Matrix = (double*)malloc(N*N * sizeof(double));
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				Matrix[i*N + j] = (i == j) ? 2 : 1;
			}

		//отдать каждому процессу его строки
		for (int i = 1; i < size_proc; i++)
		{
			MPI_Send(Matrix + i * rows*N, rows*N, MPI_DOUBLE, i, 123, MPI_COMM_WORLD);
		}
		A = Matrix;

	}
	else {
		A = (double*)malloc(rows*N * sizeof(double));
		MPI_Recv(A, rows*N, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//получить отданные строки в буфер по номеру процесса
	}

	double *x = (double*)malloc(N * sizeof(double));
	double *part_x = (double*)malloc(rows * sizeof(double));

	for (int i = 0; i < rows; i++) {
		part_x[i] = 32424325 * (i + rows * thread_id);//rand_vec[i];
	}


	double *B = (double*)malloc(rows * sizeof(double));
	for (int i = 0; i < rows; i++) {
		B[i] = N + 1;
	}
	double* y_full = (double*)malloc(N * sizeof(double));
	double* y = (double*)malloc(rows * sizeof(double));
	double* Ay_full = (double*)malloc(N * sizeof(double));
	double* Ay = (double*)malloc(rows * sizeof(double));
	double norma_b = (N + 1)*sqrt(N);

	while (1) {
		MPI_Allgather(part_x, rows, MPI_DOUBLE, x, rows, MPI_DOUBLE, MPI_COMM_WORLD);
		mulMatrix(A, x, y, rows*N); //Ax
		minusVect(y, B, y, rows); //Ax-b
		MPI_Allgather(y, rows, MPI_DOUBLE, y_full, rows, MPI_DOUBLE, MPI_COMM_WORLD);

		if (norma(y_full, N) / norma_b < Eps) {
			break;
		}
		mulMatrix(A, y_full, Ay, rows*N);
		MPI_Allgather(Ay, rows, MPI_DOUBLE, Ay_full, rows, MPI_DOUBLE, MPI_COMM_WORLD);
		double tau = ScalarMul(y_full, Ay_full, N) / ScalarMul(Ay_full, Ay_full, N);//tau
		mulScalar(y, y, tau, rows);
		minusVect(part_x, y, part_x, rows);
	}
	if (thread_id == 0) {
		//Print(x, N);
		double time_end = MPI_Wtime();
		double delta = time_end - time_begin;
		printf("Time: %lf s\n", delta);
	}
	free(Ay);
	free(Ay_full);
	free(y);
	free(y_full);
	free(B);
	free(x);
	free(A);
	MPI_Finalize();
	return 0;
}