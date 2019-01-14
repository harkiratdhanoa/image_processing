#include <bits/stdc++.h>
using std::cout;
using std::vector;

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


Matrix maxPooling(Matrix image, int stride){
    
    if(image[0].size()%stride!=0)                                             // side padding
        for(int i = 0; i<image.size();i++)
            while(image[i].size()%stride!=0)
                image[i].push_back(0.0);
    if(image.size()%stride!=0){                                              // bottom padding
        while(image.size()%stride!=0){
            row r;
            image.push_back(r);
            for(int i = 0; i < image[0].size(); i++)
                image[image.size()][i]=0.0;
        }
    }
    Matrix maxpooled;
    
    for(int i = 0; i<image.size()/stride;i++){
        row r;
        maxpooled.push_back(r);
         for(int j = 0; i<image[0].size();i++){
             maxpooled[i].push_back(image[i][j]);
              for(int t = 0; t<stride;t++)
                  for(int s = 0; s<stride;s++)
                      if(maxpooled[i][j]>image[i+t][j+s])
                          maxpooled[i][j]=image[i+t][j+s];
        }
    }
    return maxpooled;
}


Matrix avgPooling(Matrix image, int stride){
    
    if(image[0].size()%stride!=0)                                             // side padding
        for(int i = 0; i<image.size();i++)
            while(image[i].size()%stride!=0)
                image[i].push_back(0.0);
    if(image.size()%stride!=0){                                              // bottom padding
        row r;
        image.push_back(r);
        for(int i = 0; i<image[0].size();i++)
            while(image.size()%stride!=0)
                image[image.size()][i]=0.0;
    }
    Matrix avgpooled;
    
    for(int i = 0; i<image.size()/stride;i++){
        row r;
        avgpooled.push_back(r);
        for(int j = 0; i<image[0].size();i++){
            avgpooled[i].push_back(0.0);
            for(int t = 0; t<stride;t++)
                for(int s = 0; s<stride;s++)
                    avgpooled[i][j]+=image[i+t][j+s];
        }
    }
    return avgpooled;
}


int main(){
//    vector<float> a,b;
//    a.push_back(1.0);
//    a.push_back(1);
//    a.push_back(0);
//    b=softmax(a);
    Matrix image;
        row vect1;
        vect1.push_back(10.0);
        vect1.push_back(20.0);
        vect1.push_back(30.0);
        image.push_back(vect1);
        row vect2;
        vect2.push_back(40.0);
        vect2.push_back(50.0);
        vect2.push_back(60.0);
        image.push_back(vect2);
        row vect3;
        vect3.push_back(70.0);
        vect3.push_back(80.0);
        vect3.push_back(90.0);
        image.push_back(vect3);
    
    cout<<"hii";
    Matrix outt = maxPooling(image,2);
    
    for(int i=0;i<outt.size();i++){
        for(int j=0;j<outt[0].size();j++)
        cout << outt[i][j] << " ";
        cout << "\n";
    }
}


