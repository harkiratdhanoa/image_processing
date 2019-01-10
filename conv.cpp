#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;



Matrix scale(Matrix image)
{
	float max = image[0][0];
	float min = image[0][0];
	for(int i = 0; i<image.size();i++){
		for(int j = 0; j<image[0].size(); j++){
			if(max<image[i][j])
				max = image[i][j];
			if(min>image[i][j])
				min = image[i][j];
		}
	}
	float scale = 255/(max -min);
	for(int i = 0; i<image.size();i++){
		for(int j = 0; j<image[0].size(); j++){
			image[i][j] = image[i][j] - min;
			image[i][j] = floor(image[i][j]*scale);
		}
	}
	return image;
}

Matrix convolution(Matrix image, Matrix filter, bool padded)
{
	int image_row_no = image.size();
	int image_column_no = image[0].size();
	int filter_size = filter.size();
	
	
	int border = (filter_size-1)/2;
	Matrix result(image_row_no-2*border, row(image_column_no-2*border));
	
	if(padded)
	{
		Matrix padded_image(image_row_no+2*border, row(image_column_no+2*border));
		for(int i = 0; i<image_row_no+2*border; i++){
			for(int j = 0; j<image_column_no+2*border;j++){
				if(i<border || i>image_row_no - border+1||j<border || j>image_column_no - border+1)
					padded_image[i][j] = 0;
				else
					padded_image[i][j] = image[i-border][j-border];
			}
		}
		image_row_no = image_row_no + 2*border;
		image_column_no = image_column_no + 2*border;
		Matrix result(image_row_no-2*border, row(image_column_no-2*border));
		cout<< result[1][2];
		for(int i = 0; i<image_row_no-2*border; i++){
			for(int j = 0; j<image_column_no-2*border;j++){
				for(int k = 0; k<filter_size; k++){
					for(int l = 0; l<filter_size;l++){
						result[i][j] = result[i][j] + padded_image[i+k][j+l]*filter[filter_size-k-1][filter_size-l-1];
					}
				}
			}
		}
		result = scale(result);
		return result;
	}
	
	else
	{
		for(int i = 0; i<image_row_no-2*border; i++){
			for(int j = 0; j<image_column_no-2*border;j++){
				for(int k = 0; k<filter_size; k++){
					for(int l = 0; l<filter_size;l++){
						result[i][j] = result[i][j] + image[i+k][j+l]*filter[filter_size-k-1][filter_size-l-1];
					}
				}
			}
		}
	
		result = scale(result);
		return result;
	}
}

Matrix nonlinear(Matrix image, bool select)
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
	int rows;
	int columns;
	ifstream myfile;
	myfile.open("image.txt");
	myfile >> rows;
	myfile >> columns;
	
	Matrix image(rows, row(columns));
	for(int j = 0; j<columns; j++){
		for(int i = 0; i<rows; i++){
			myfile >> image[i][j];
		}
	}
	myfile.close();
	
	Matrix filter(3, row(3));
	filter[0][0] = 1.0/16;
	filter[0][1] = 2.0/16;
	filter[0][2] = 1.0/16;
	filter[1][0] = 2.0/16;
	filter[1][1] = 4.0/16;
	filter[1][2] = 2.0/16;
	filter[2][0] = 1.0/16;
	filter[2][1] = 2.0/16;
	filter[2][2] = 1.0/16;
	Matrix result = convolution(image, filter, true);
	
	ofstream outfile;
	outfile.open("fil_image.txt");
	for(int j = 0; j<result[0].size(); j++){
		for(int i = 0; i<result.size(); i++){
			outfile << result[i][j];
			outfile << " ";
		}
		outfile << "\n";
	}
	outfile.close();
	
	return 0;
}