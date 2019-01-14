#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "NonLinear.h"
#include "Convolution.h"
#include "Read.h"
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

int main(int argc, char** argv)
{
	int count = 1;
	string function = argv[count++];
	
	try{
		Matrix result;
		if(function.compare("convolution") == 0)
		{
		
			Matrix matrix1 = Matrix_Read(string(argv[count++]));
			Matrix matrix2 = Matrix_Read(string(argv[count++]));
		
			if((string(argv[count])).compare("padding")==0)
			{
				count++;
				if((string(argv[count])).compare("convolution") == 0)
					result = convolution(&matrix1, &matrix2, true, false);
				else if((string(argv[count])).compare("matrix_mult")==0)
					result = convolution(&matrix1, &matrix2, true, true);
				else
					cout << "Please Enter 'convolution'  or 'matrix_mut'";
			}
			else if((string(argv[count])).compare("no_padding")==0)
			{
				count++;
				if((string(argv[count])).compare("convolution")==0)
					result = convolution(&matrix1, &matrix2, false, false);
				else if((string(argv[count])).compare("matrix_mult")==0)
					result = convolution(&matrix1, &matrix2, false, true);
				else
					cout << "Please Enter 'convolution'  or 'matrix_mut'";
			}
			else
				cout << "Please Enter 'padding'  or 'no_padding'";
		}
		else if(function.compare("non_linear") == 0)
		{
			Matrix matrix1 = Matrix_Read(string(argv[count++]));
			if((string(argv[count])).compare("relu")==0)
			{
				result = NonLinear(&matrix1, true);
			}
			else if((string(argv[count])).compare("tanh")==0)
			{
				result = NonLinear(&matrix1, false);
			}
			else
				cout << "Please Enter 'relu'  or 'tanh'";
		}
	
		else
		{
			cout << "Please enter a valid function name\n";
		}
	
	
	
	
	
	
	
		for(int i = 0; i<result.size(); i++){
			for(int j = 0; j<result[0].size(); j++){
				cout << result[i][j];
				cout << " ";
			}
			cout << "\n";
		}
	}
	catch(int e)
	{
		cout << "File does not exist\n";
		return 0;
	}
	return 0;
}