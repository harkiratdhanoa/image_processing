#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;


Matrix NonLinear(Matrix* image, bool select)
{
	Matrix result((*image).size(), row((*image)[0].size()));
	if(select)	  //if select is true, perform relu
	{
		for(int i = 0; i<(*image).size();i++){
			for(int j = 0; j<(*image)[0].size(); j++){
				if((*image)[i][j] < 0)		//filter out all negative values
					result[i][j] = 0;
				else
					result[i][j] = (*image)[i][j];
			}
		}
	}
	else	//if select is false, perform tanh
	{
		for(int i = 0; i<(*image).size();i++){
			for(int j = 0; j<(*image)[0].size(); j++){
				result[i][j] = tanh((*image)[i][j]);	//take tanh
			}
		}
	}
	return result;
}