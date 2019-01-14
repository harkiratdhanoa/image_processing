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
	if(!myfile.fail())
	{
		while((myfile>>temp)){
			count++;	//count the total numbers expected
		}
		myfile.close();
		ifstream myfile2;
		myfile2.open(filename);
		int size = (int)sqrt((double)count);	//assuming square matrix, size is root of total numbers 
		Matrix m(size, row(size));		//construct the matrix
		for(int j = 0; j<size; j++){		//begin reading the values into the matrix
			for(int i = 0; i<size; i++){
				myfile2 >> m[i][j];
			}
		}
		myfile2.close();
		return m;
	}
	else
		throw 20;
}

Vector Vector_Read(string filename)
{
	float temp;
	ifstream myfile;
	myfile.open(filename);
	Vector v;
	if(!myfile.fail()){
		while(myfile >> temp){
			v.push_back(temp);	//keep adding the reaed numbers (using dynamic property of C++ vector
		}
		myfile.close();
		return v;
	}
	else
		throw 20;
}
