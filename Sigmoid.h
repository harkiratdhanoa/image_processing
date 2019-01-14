#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;
typedef vector<float> row;
typedef vector<row> Matrix;

vector<float> sigmoid(vector<float> v){
	int i;
	vector<float> v_out;
	for(i=0;i<v.size();i++)
		v_out.push_back(1.0/(1.0+exp(-v[i])));
	return v_out;
}


vector<float> softmax(vector<float> v){
    float sum=0.0;
    vector<float> v_out;
    for(int i=0;i<v.size();i++){
        v_out.push_back(exp(v[i]));
        sum+=v_out[i];
    }
    for(int i=0;i<v.size();i++){        // normalize
        v_out[i]/=sum;
    }
    return v_out;
}