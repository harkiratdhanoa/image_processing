#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

int main(int argc, char** argv)
{
	ofstream myfile;
	int count;
	try{
        count = std::stoi(string(argv[1]));
    }
    catch(std::invalid_argument){
    	cout<<"Error";
        return 0;
    }
	myfile.open("matrix.txt");
	for(int i = 0; i<count*count;i++)
		myfile << (((float)rand())/((float) RAND_MAX)) << " ";;
	myfile.close();
}