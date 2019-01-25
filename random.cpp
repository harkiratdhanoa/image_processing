#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <chrono>
#include "Convolution.h"
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

Matrix RandomMatrix(int size)
{
	Matrix random(size, row(size));
	for(int i = 0; i<size;i++)
		for(int j = 0;j<size; j++)
			random[i][j] = (((float)rand())/((float) RAND_MAX) * 100) -5;
	return random;		
}

int main()
{
	Matrix image;
	Matrix filter;
	Matrix result;

	for(int image_size = 4; image_size<600; image_size = image_size*2)
	{
		image = RandomMatrix(image_size);
		for(int filter_size = 2; filter_size < 33; filter_size = filter_size*2)
		{
			filter = RandomMatrix(filter_size);
			for(int k = 0; k<20; k++)
			{
				convolution(&image, &filter, 1, 3);
			}
		}
	}
	return 0;
}
		