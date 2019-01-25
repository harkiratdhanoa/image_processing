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

Matrix Image_Mult_Form(Matrix* image, int filter_size)	//Prepare the Image Matrix for Convolution by Multiplication
{
	int rows = (*image).size();		//rows of image
	int columns = (*image)[0].size();	//columns of image
	int new_rows = (rows-filter_size+1) * (columns-filter_size+1);	//rows of canonical matrix we expect
	int new_columns = filter_size*filter_size;	//columns of canonical matrix we expect
	Matrix image_mult_form(new_rows, row(new_columns));		//construct the cononical matrix
	
	int row; 
	int column;
	//I am mapping each element (except the last few due to border constraints) to a row in can.
	//I then map the other elements in that same row in terms of a row and column offset
	int offset_row;		
	int offset_column;
	for(int i = 0; i<new_rows; i++){
		row = (int)(i/(rows-filter_size+1)); 
		column = i%(columns-filter_size+1);
		for(int j = 0; j<new_columns; j++){
			offset_row = (int)(j/filter_size);
			offset_column = j%filter_size;
			image_mult_form[i][j] = (*image)[row + offset_row][column+offset_column];
		}
	}
	return image_mult_form;
}

Matrix Filter_Mult_Form(Matrix* filter)	//Prepare the Filter Matrix for Convolution by Multiplication
{
	int size = (*filter).size();
	Matrix filter_mult_form(size*size, row(1));
	for(int i = 0; i<size; i++){
		for(int j = 0; j<size; j++){
			filter_mult_form[((size-i-1)*size) + (size-j-1)][0] = (*filter)[i][j];
		}
	}
	return filter_mult_form;
}

Matrix Reverse_Mat_Mult_Form(Matrix* image, int rows, int columns) //Reconstruct the Final Matrix after Multiplication and Adding
{
	Matrix result(rows, row(columns));
	int count = 0;
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<columns; j++){
			result[i][j] = (*image)[count][0];
			count++;
		}
	}
	return result;
}

Matrix Matrix_Mult(Matrix* m1, Matrix* m2) //My definition of simple Matrix Multiplication
{
	Matrix result((*m1).size(), row(1));
	for(int i = 0; i<(*m1).size(); i++){
		for(int k = 0; k<(*m1)[0].size(); k++){
			result[i][0]+=(*m1)[i][k]*(*m2)[k][0];
		}
	}
	
	return result;
}

Matrix Padded_Image(Matrix* image, int filter_size, int image_row_no, int image_column_no, int custom_border_size=0) //Construct a Padded Image, based on filter size
{
	int border;
    if(custom_border_size>0) //for custom_padding
    {
        if(filter_size%2 == 1)	//if filter_size is odd
        {
            border = custom_border_size;
            //add border of zeroes on all sides
            Matrix padded_image(image_row_no + 2*border, row(image_column_no  + 2*border));
            for(int i = 0; i<image_row_no + 2*border; i++){
                for(int j = 0; j<image_column_no + 2*border;j++){
                    if(i<border || i>image_row_no + border-1||j<border || j>image_column_no + border-1)
                        padded_image[i][j] = 0;
                    else
                        padded_image[i][j] = (*image)[i-border][j-border];
                }
            }
            return padded_image;
        }
        else	//filter_size is even
        {
            border = custom_border_size;
            //add border of zeroes on the bottom and right; but add border -1 on top and left
            Matrix padded_image(image_row_no + 2*border-1, row(image_column_no + 2*border-1));
            for(int i = 0; i<image_row_no + 2*border-1; i++){
                for(int j = 0; j<image_column_no + 2*border-1;j++){
                    if(i<border-1||i>image_row_no+border-2||j>image_column_no+border-2||j<border-1)
                        padded_image[i][j] = 0;
                    else
                        padded_image[i][j] = (*image)[i-border+1][j-border+1];
                }
            }
            return padded_image;
        }
    }
    //for normal same size padding
	if(filter_size%2 == 1)	//if filter_size is odd
	{
		border = (filter_size-1)/2;
		//add border of zeroes on all sides
		Matrix padded_image(image_row_no + 2*border, row(image_column_no  + 2*border));
		for(int i = 0; i<image_row_no + 2*border; i++){
			for(int j = 0; j<image_column_no + 2*border;j++){
				if(i<border || i>image_row_no + border-1||j<border || j>image_column_no + border-1)
				padded_image[i][j] = 0;
				else
					padded_image[i][j] = (*image)[i-border][j-border];
			}
		}
		return padded_image;
	}
	else	//filter_size is even
	{
		border = filter_size/2;
		//add border of zeroes on the bottom and right; but add border -1 on top and left
		Matrix padded_image(image_row_no + 2*border-1, row(image_column_no + 2*border-1));
		for(int i = 0; i<image_row_no + 2*border-1; i++){
			for(int j = 0; j<image_column_no + 2*border-1;j++){
				if(i<border-1||i>image_row_no+border-2||j>image_column_no+border-2||j<border-1)
					padded_image[i][j] = 0;
				else
					padded_image[i][j] = (*image)[i-border+1][j-border+1];
			}
		}
		return padded_image;
	}
	
}

Matrix convolution(Matrix* image, Matrix* filter, int padded=1, int mult = 1, int custom_border_size = 0)
{
	int image_row_no = (*image).size();
	int needed = (*image).size();
	int image_column_no = (*image)[0].size();
	int filter_size = (*filter).size();
	
	Matrix i;
	int border;		//compute border size
	if(filter_size%2 == 1)
		border = (filter_size-1)/2;
	else
		border = filter_size/2;
	
	if(padded==1)	//if padded is 1, perform same size padding
	{
		i = Padded_Image(image, filter_size, image_row_no, image_column_no);
		image = &i;
		image_row_no = (*image).size();		//update row number
		image_column_no = (*image)[0].size();		//update column number
	}
	else if(padded == 2)    //if padded is 2, perform custom padding
    {
        i = Padded_Image(image, filter_size, image_row_no, image_column_no, custom_border_size);
        image = &i;
        image_row_no = (*image).size();		//update row number
        image_column_no = (*image)[0].size();		//update column number
    }
    
	Matrix result(image_row_no-filter_size+1, row(image_column_no-filter_size+1));		//Initialise a matrix of size n-f+1
	
	if(mult == 1) 	//use matrix multiplication method
	{	
		Matrix Image = Image_Mult_Form(image, filter_size);
		Matrix Filter = Filter_Mult_Form(filter);
		
		auto start = std::chrono::high_resolution_clock::now();
		Matrix product = Matrix_Mult(&Image, &Filter);	//Perform Matrix Multiplication
		auto finish = std::chrono::high_resolution_clock::now();
		result = Reverse_Mat_Mult_Form(&product, image_row_no-filter_size+1, image_row_no-filter_size+1);	//Reconstruct the final matrix	
		std::chrono::duration<double> elapsed = finish - start;
		cout << elapsed.count()<<"\n";
		
	}
	else if(mult == 2) //mkl
	{
		double* Image = Image_MKL_Mult_Form(image, filter_size);
		double* Filter = Filter_MKL_Mult_Form(filter);
		double* product = new double[(image_row_no-filter_size+1)*(image_row_no-filter_size+1)];
		auto start = std::chrono::high_resolution_clock::now();
		
		dgemm_mkl((image_row_no-filter_size+1)*(image_row_no-filter_size+1),  1, filter_size*filter_size,1, Image,  (image_row_no-filter_size+1)*(image_row_no-filter_size+1), Filter, filter_size*filter_size, 0, product, (image_row_no-filter_size+1)*(image_row_no-filter_size+1));	//Perform Matrix Multiplication
		auto finish = std::chrono::high_resolution_clock::now();
		result = Reverse_MKL_Mult_Form(product, image_row_no-filter_size+1, image_row_no-filter_size+1);	//Reconstruct the final matrix
		std::chrono::duration<double> elapsed = finish - start;
		cout << elapsed.count()<<"\n";
	}
	else if(mult == 3) //openblas
	{
		double* Image = Image_MKL_Mult_Form(image, filter_size);
		double* Filter = Filter_MKL_Mult_Form(filter);
		double* product = new double[(image_row_no-filter_size+1)*(image_row_no-filter_size+1)];
		auto start = std::chrono::high_resolution_clock::now();
		
		dgemm_blas((image_row_no-filter_size+1)*(image_row_no-filter_size+1),  1, filter_size*filter_size,1, Image,  (image_row_no-filter_size+1)*(image_row_no-filter_size+1), Filter, filter_size*filter_size, 0, product, (image_row_no-filter_size+1)*(image_row_no-filter_size+1));	//Perform Matrix Multiplication
		auto finish = std::chrono::high_resolution_clock::now();
		result = Reverse_MKL_Mult_Form(product, image_row_no-filter_size+1, image_row_no-filter_size+1);	//Reconstruct the final matrix
		
		
		std::chrono::duration<double> elapsed = finish - start;
		cout << elapsed.count()<<"\n";
	}
	else	//use normal convolution 
	{	
		auto start = std::chrono::high_resolution_clock::now();
		for(int i = 0; i<image_row_no-filter_size+1; i++){
			for(int j = 0; j<image_row_no-filter_size+1;j++){
				for(int k = 0; k<filter_size; k++){
					for(int l = 0; l<filter_size;l++){
						result[i][j] = result[i][j] + (*image)[i+k][j+l]* (*filter)[filter_size-k-1][filter_size-l-1];
					}
				}
			}
		}
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		cout << elapsed.count()<<"\n";
		
	}
	return result;	
}
