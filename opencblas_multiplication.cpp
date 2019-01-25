#include "cblas.h"
#include <iostream>

void dgemm_blas(int m, int n, int k, int alpha, double* A, int mm, double* B, int kk, int beta, double* C, int mmm)
{
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k,alpha, A, mm, B, kk, beta, C, mmm);	//Perform Matrix Multiplication
	return;
}
