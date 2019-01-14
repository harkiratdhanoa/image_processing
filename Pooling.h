#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;
typedef vector<float> row;
typedef vector<row> Matrix;

Matrix maxPooling(Matrix image, int stride){
    
    if(image[0].size()%stride!=0)                                             // side padding
        for(int i = 0; i<image.size();i++)
            while(image[i].size()%stride!=0)
                image[i].push_back(0.0);
    if(image.size()%stride!=0){                                              // bottom padding
        while(image.size()%stride!=0){
            row r(image[0].size());
            image.push_back(r);
            //for(int i = 0; i < image[0].size(); i++)
              //	image[image.size()-1][i]=0.0;
        }
    }
   Matrix maxpooled;
    for(int i = 0; i<image.size()/stride;i++){
        row r(image[0].size()/stride);
        maxpooled.push_back(r);
         for(int j = 0; j<image[0].size()/stride;j++){
             maxpooled[i][j]=(image[i*stride][j*stride]);
              for(int t = 0; t<stride;t++)
                  for(int s = 0; s<stride;s++)
                      if(maxpooled[i][j]<image[i*stride+t][j*stride+s])
                          maxpooled[i][j]=image[i*stride+t][j*stride+s];
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
    	while(image.size()%stride!=0)
        {
        	row r(image[0].size());
        	image.push_back(r);
        	//for(int i = 0; i<image[0].size();i++)
            	//image[image.size()][i]=0.0;
        }
    }
    Matrix avgpooled;
    
    for(int i = 0; i<image.size()/stride;i++){
        row r(image[0].size()/stride);
        avgpooled.push_back(r);
        for(int j = 0; j<image[0].size();j++){
            avgpooled[i][j] = 0;
            for(int t = 0; t<stride;t++)
                for(int s = 0; s<stride;s++)
                    avgpooled[i][j]+=image[i*stride+t][j*stride+s];
            avgpooled[i][j] = avgpooled[i][j]/(stride*stride);
        }
    }
    return avgpooled;
}