#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <chrono>
#include "mkl_mult.h"
#include "mkl_multiplication.h"
#include "opencblas_multiplication.h"

using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;
Matrix Image_Mult_Form(Matrix* image, int filter_size);
Matrix Filter_Mult_Form(Matrix* filter);	
Matrix Reverse_Mat_Mult_Form(Matrix* image, int rows, int columns);
Matrix Matrix_Mult(Matrix* m1, Matrix* m2);
Matrix Padded_Image(Matrix* image, int filter_size, int image_row_no, int image_column_no, int custom_border_size);
Matrix convolution(Matrix* image, Matrix* filter, int padded, int mult, int custom_border_size=0);