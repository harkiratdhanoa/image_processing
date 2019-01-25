#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <chrono>

using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

double* Image_MKL_Mult_Form(Matrix* image, int filter_size);
double* Filter_MKL_Mult_Form(Matrix* filter);
Matrix Reverse_MKL_Mult_Form(double* image, int rows, int columns);
