#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
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
	Matrix result((*m1).size(), row((*m2)[0].size()));
	for(int i = 0; i<(*m1).size(); i++){
		for(int j = 0; j<(*m2)[0].size(); j++){
			for(int k = 0; k<(*m1)[0].size(); k++){
				result[i][j]+=(*m1)[i][k]*(*m2)[k][j];
			}
		}
	}
	return result;
}

Matrix Padded_Image(Matrix* image, int filter_size, int image_row_no, int image_column_no) //Construct a Padded Image, based on filter size
{
	int border;
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

Matrix convolution(Matrix* image, Matrix* filter, bool padded = true, bool mult = false)
{
	int image_row_no = (*image).size();
	int image_column_no = (*image)[0].size();
	int filter_size = (*filter).size();
	
	Matrix i;
	int border;
	if(filter_size%2 == 1)
		border = (filter_size-1)/2;
	else
		border = filter_size/2;
	
	if(padded)
	{
		i = Padded_Image(image, filter_size, image_row_no, image_column_no);
		image = &i;
		image_row_no = (*image).size();
		image_column_no = (*image)[0].size();
	}
	
	
	
	Matrix result(image_row_no-filter_size+1, row(image_column_no-filter_size+1));
	
	if(mult)
	{
		Matrix Image = Image_Mult_Form(image, filter_size);
		Matrix Filter = Filter_Mult_Form(filter);
		Matrix product = Matrix_Mult(&Image, &Filter);
		Matrix sum_product(product.size(), row(1));
		for(int i = 0; i<product.size(); i++){
			for(int j = 0; j<product[0].size(); j++){
				sum_product[i][0] += product[i][j];
			}
		}
		result = Reverse_Mat_Mult_Form(&sum_product, image_row_no-filter_size+1, image_row_no-filter_size+1);
	}
	else
	{
		for(int i = 0; i<image_row_no-filter_size+1; i++){
			for(int j = 0; j<image_row_no-filter_size+1;j++){
				for(int k = 0; k<filter_size; k++){
					for(int l = 0; l<filter_size;l++){
						result[i][j] = result[i][j] + (*image)[i+k][j+l]* (*filter)[filter_size-k-1][filter_size-l-1];
					}
				}
			}
		}
	}
	return result;	
}