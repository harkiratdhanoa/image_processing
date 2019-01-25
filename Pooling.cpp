#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;
typedef vector<float> row;
typedef vector<row> Matrix;

Matrix Pooling(Matrix image, bool select, int pool_size, int stride){
    int rows = image.size()/stride+1;	//predict the rows in the pooled output matrix
    int columns = image[0].size()/stride+1;		//predict the columns in the pooled output matrix
    
    Matrix copy(image.size(), row(image[0].size())); //we preserve the original image and create a copy for manipulation
    for(int i = 0; i<image.size(); i++){
    	for(int j = 0; j<image[0].size();j++){
    		copy[i][j] = image[i][j];
    	}
    }
    
    int extra = copy[0].size()%stride; //pad on the RIGHT with zeroes to fit window size
    if(extra == 0){
    	columns--;
    	extra = stride;
    }
    while(extra!=pool_size){
    	for(int i = 0; i<copy.size();i++)
    		copy[i].push_back(0.0);
    	extra++;
    }
    
    extra = copy.size()%stride; //pad on the BOTTOM with zeroes to fit the window size
    if(extra == 0){
    	rows--;
    	extra = stride;
    }
    while(extra!=pool_size)
    {
    	row r(copy[0].size());
        copy.push_back(r);
    	extra++;
    }

	if(select)
	{
   		Matrix maxpooled;
		for(int i = 0; i<rows;i++){	//Initialise the pooled matrix with zeroes
        	row r(columns);
    		maxpooled.push_back(r);
        	for(int j = 0; j<columns;j++){
             	maxpooled[i][j]=(copy[i*stride][j*stride]);
              	for(int t = 0; t<pool_size;t++)
                  	for(int s = 0; s<pool_size;s++)
                      	if(maxpooled[i][j]<copy[i*stride+t][j*stride+s])		//check for maximum within pool window
                          	maxpooled[i][j]=copy[i*stride+t][j*stride+s];
        	}
    	}
    	return maxpooled;
    }
    else
    {
    	Matrix avgpooled;
    	for(int i = 0; i<rows;i++){	 //Initialise the pooled matrix with zeroes
        	row r(columns);
        	avgpooled.push_back(r);
        	for(int j = 0; j<columns;j++){
            	avgpooled[i][j] = 0;
            	for(int t = 0; t<pool_size;t++){
                	for(int s = 0; s<pool_size;s++){
                	    avgpooled[i][j]+=copy[i*stride+t][j*stride+s];		//take sum within pool window
                	}
            	}
            	avgpooled[i][j] = avgpooled[i][j]/(pool_size*pool_size);		//divide sum by stride window size for average
        	}
    	}
    	return avgpooled;
    }
}


