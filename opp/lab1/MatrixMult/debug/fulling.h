#include <stdlib.h>

void full_1(double* A, double* b, double* x, size_t size);
void full_2(double* A, double* b, double* x, size_t size);

void VECTxSCAL(double* res, double* vect, double scal, size_t size);
void MATxVECT(double* res, double* mat, double* vect, size_t size);
void VECTsubVECT(double* res, double* vect1, double* vect2, size_t size);
