#include <bits/stdc++.h>
using std::cout;
using std::vector;


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


//int main(){
//    vector<float> a,b;
//    a.push_back(1.0);
//    a.push_back(1);
//    a.push_back(0);
//    b=softmax(a);
//    for(int i=0;i<b.size();i++){
//        cout << b[i] << " ";
//    }
//}
