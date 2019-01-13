#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

Matrix Matrix_Read(string filename)
{
	ifstream myfile;
	myfile.open(filename);
	int count = 0;
	float temp;
	while((myfile>>temp)){
		count++;
	}
	myfile.close();
	ifstream myfile2;
	myfile2.open(filename);
	int size = (int)sqrt((double)count);
	Matrix m(size, row(size)); 
	for(int j = 0; j<size; j++){
		for(int i = 0; i<size; i++){
			myfile2 >> m[i][j];
		}
	}
	myfile2.close();
	return m;
}

Vector Vector_Read(string filename)
{
	float temp;
	ifstream myfile;
	myfile.open(filename);
	Vector v;
	while(myfile >> temp){
		v.push_back(temp);
	}
	myfile.close();
	return v;
}


Matrix Image_Mult_Form(Matrix* image, int filter_size)
{
	int rows = (*image).size();
	int columns = (*image)[0].size();
	int new_rows = (rows-filter_size+1) * (columns-filter_size+1);
	int new_columns = filter_size*filter_size;
	Matrix image_mult_form(new_rows, row(new_columns));
	
	int row; 
	int column;
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

Matrix Filter_Mult_Form(Matrix* filter)
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

Matrix Reverse_Mat_Mult_Form(Matrix* image, int rows, int columns)
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

Matrix Matrix_Mult(Matrix* m1, Matrix* m2)
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

Matrix Padded_Image(Matrix* image, int border, int image_row_no, int image_column_no)
{
	Matrix padded_image(image_row_no, row(image_column_no));
	for(int i = 0; i<image_row_no; i++){
		for(int j = 0; j<image_column_no;j++){
			if(i<border || i>image_row_no - border-1||j<border || j>image_column_no - border-1)
				padded_image[i][j] = 0;
			else
				padded_image[i][j] = (*image)[i-border][j-border];
		}
	}
	return padded_image;
}

Matrix convolution(Matrix* image, Matrix* filter, bool padded = true, bool mult = false)
{
	int image_row_no = (*image).size();
	int image_column_no = (*image)[0].size();
	int filter_size = (*filter).size();
	
	Matrix i;
	
	int border = (filter_size-1)/2;
	
	if(padded)
	{
		image_row_no = image_row_no + 2*border;
		image_column_no = image_column_no + 2*border;
		i = Padded_Image(image, border, image_row_no, image_column_no);
		image = &i;
	}
	
	Matrix result(image_row_no-2*border, row(image_column_no-2*border));
	
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
		result = Reverse_Mat_Mult_Form(&sum_product, image_row_no-2*border, image_column_no-2*border);
	}
	else
	{
		for(int i = border; i<image_row_no-border; i++){
			for(int j = border; j<image_column_no-border;j++){
				for(int k = -border; k<border+1; k++){
					for(int l = -border; l<border+1;l++){
						result[i-border][j-border] = result[i-border][j-border] + (*image)[i+k][j+l]* (*filter)[-k+border][-l+border];
					}
				}
			}
		}
	}
	return result;	
}

Matrix NonLinear(Matrix image, bool select)
{
	if(select)
	{
		for(int i = 0; i<image.size();i++){
			for(int j = 0; j<image[0].size(); j++){
				if(image[i][j] < 0)
					image[i][j] = 0;
			}
		}
	}
	else
	{
		for(int i = 0; i<image.size();i++){
			for(int j = 0; j<image[0].size(); j++){
				image[i][j] = tanh(image[i][j]);
			}
		}
	}
	return image;
}

int main()
{
	Matrix m = Matrix_Read("image.txt");
	Matrix m2 = Matrix_Read("filter.txt");
	
	Matrix result = convolution(&m, &m2, true, false);
	
	for(int i = 0; i<result.size(); i++){
		for(int j = 0; j<result[0].size(); j++){
			cout << result[i][j];
			cout << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	for(int i = 0; i<m.size(); i++){
		for(int j = 0; j<m[0].size(); j++){
			cout << m[i][j];
			cout << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	for(int i = 0; i<m2[0].size(); i++){
		for(int j = 0; j<m2.size(); j++){
			cout << m2[i][j];
			cout << " ";
		}
		cout << "\n";
	}
	
	
	return 0;
}