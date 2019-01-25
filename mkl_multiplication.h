using namespace std;

void dgemm_mkl(int m, int n, int k, int alpha, double* A, int mm, double* B, int kk, int beta, double* C, int mmm);