#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "NonLinear.h"
#include "Convolution.h"
#include "Read.h"
#include "Sigmoid.h"
#include "Pooling.h"
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

int main(int argc, char** argv)
{
	int count = 1;
	string function = argv[count++];
	
	try{
		
		if(function.compare("squash") == 0)
		{
			if(argc != 4){
				cout << "Please enter all arguments: squash filename sigmoid/softmax\n";
				return 0;
			}
			Vector result;
			Vector v = Vector_Read(string(argv[count++]));
			if((string(argv[count])).compare("sigmoid")==0)
			{
				result = sigmoid(v);
			}
			else if((string(argv[count])).compare("softmax")==0)
			{
				result = softmax(v);
			}
			else
			{
				cout << "Please Enter 'sigmoid'  or 'softmax'\n";
				return 0;
			}
			for(int i = 0; i<result.size(); i++){
				cout << result[i];
				cout << " ";
			}
			cout << "\n";
			return 0;
		}
			
		
		
		Matrix result;
		if(function.compare("convolution") == 0)
		{
			if(argc != 6){
				cout << "Please enter all arguments: convolution filename1 filename2 padding/no_padding convolution/matrix_mul\n";
				return 0;
			}
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
					cout << "Please Enter 'convolution'  or 'matrix_mult'";
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
			if(argc != 4){
				cout << "Please enter all arguments: non_linear filename relu/tanh\n";
				return 0;
			}
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
		else if(function.compare("pooling") == 0)
		{
			if(argc != 5){
				cout << "Please enter all arguments: pooling filename max/avg stride\n";
				return 0;
			}
			Matrix matrix1 = Matrix_Read(string(argv[count++]));
			if((string(argv[count])).compare("max")==0)
			{
				count++;
				if(count < argc)
				{	
					int stride;
					try{
						stride = std::stoi(string(argv[count]));
					}
					catch(std::invalid_argument){
						cout << "Please enter valid stride\n";
						return 0;
					}
					if(stride > matrix1.size()){
						cout << "Please enter valid stride\n";
						return 0;
					}
					result = maxPooling(matrix1, stride);
				}
				else
					cout << "Please enter an integer stride\n";
			}
			else if((string(argv[count])).compare("average")==0)
			{
				count++;
				if(count < argc)
				{	
					int stride;
					try{
						stride = std::stoi(string(argv[count]));
					}
					catch(std::invalid_argument){
						cout << "Please enter valid stride\n";
						return 0;
					}
					if(stride > matrix1.size()){
						cout << "Please enter valid stride\n";
						return 0;
					}
					result = avgPooling(matrix1, stride);
				}
				else
					cout << "Please enter an integer stride\n";
			}
			else
				cout << "Please Enter 'max'  or 'average\n'";
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