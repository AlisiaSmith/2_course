#include <stdlib.h>

#include "fulling.h"


void full_1(double* A, double* b, double* x, size_t size)
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			A[i*size + j] = (i == j) ? 2.0 : 1.0;

	for (int i = 0; i < size; ++i)
	{
		b[i] = size + 1;
		x[i] = 0;
	}
}

void full_2(double* A, double* b, double* x, size_t size)
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
