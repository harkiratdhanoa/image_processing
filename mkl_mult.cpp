#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <chrono>

using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

double* Image_MKL_Mult_Form(Matrix* image, int filter_size)
{
	int rows = (*image).size();		//rows of image
	int columns = (*image)[0].size();	//columns of image
	int new_rows = (rows-filter_size+1) * (columns-filter_size+1);	//rows of canonical matrix we expect
	int new_columns = filter_size*filter_size;	//columns of canonical matrix we expect
	
	int row; 
	int column;
	int offset_row;		
	int offset_column;
	double* Image = new double[new_rows*new_columns];
	for(int i = 0; i<new_rows; i++){
		row = (int)(i/(rows-filter_size+1)); 
		column = i%(columns-filter_size+1);
		for(int j = 0; j<new_columns; j++){
			offset_row = (int)(j/filter_size);
			offset_column = j%filter_size;
			Image[j*new_rows + i] = (double)((*image)[row + offset_row][column+offset_column]);
		}
	}
	return Image;
}

double* Filter_MKL_Mult_Form(Matrix* filter)	//Prepare the Filter Matrix for Convolution by Multiplication
{
	int size = (*filter).size();
	double* Filter = new double[size*size];
	for(int i = 0; i<size; i++){
		for(int j = 0; j<size; j++){
			Filter[((size-i-1)*size) + (size-j-1)] = (double)((*filter)[i][j]);
		}
	}
	return Filter;
}

Matrix Reverse_MKL_Mult_Form(double* image, int rows, int columns)
{
	Matrix result(rows, row(columns));
	int count = 0;
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			result[i][j] = image[count];
			count++;
		}
	}
	return result;
}

/*int main()
{
	Matrix image(7, row(7));
	Matrix filter(3, row(3));
	for(int i = 0; i<7; i++)
		for(int j = 0; j<7; j++)
			image[i][j] = i*7 +j;
	for(int i = 0; i<3;i++)
		for(int j = 0; j<3;j++)
			filter[i][j] = i*3+j;
			
	double Image[5*5*3*3];
	Image_MKL_Mult_Form(&image, 3, Image);
	double Filter[3*3];
	Filter_MKL_Mult_Form(&filter, Filter);
	double product[25];
	
	//for(int i = 0; i<25*9;i++)
	//	cout << Image[i] << " ";
	//for(int i = 0; i<9;i++)
	//	cout << Filter[i] << " ";
	
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, 25, 1, 9, 23, Image, 25, Filter, 9, 0, product, 25);	//Perform Matrix Multiplication
}*/