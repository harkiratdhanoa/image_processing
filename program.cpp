#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "Pooling.h"
#include "NonLinear.h"
#include "Convolution.h"
#include "Sigmoid.h"
#include "Read.h"
using namespace std;

typedef vector<float> row;
typedef vector<row> Matrix;
typedef vector<float> Vector;

int main(int argc, char** argv)
{
    int count = 1;	//ignore ./program
    if(argc == 1){
        cout << "Please enter a function name\n";
        return 0;
    }
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
            for(int i = 0; i<result.size(); i++){ //Print and Return
                cout << result[i];
                cout << " ";
            }
            cout << "\n";
            return 0;
        }
        
        
        
        Matrix result;
        if(function.compare("convolution") == 0)
        {
            int padsize;
            if(argc != 6 && argc !=7){
                cout << "Please enter all arguments: convolution filename1 filename2 padding/no_padding/custom_padding convolution/matrix_mult\n";
                return 0;
            }
            Matrix matrix1 = Matrix_Read(string(argv[count++]));
            Matrix matrix2 = Matrix_Read(string(argv[count++]));
            
            if((string(argv[count])).compare("padding")==0)
            {
                count++;
                if((string(argv[count])).compare("convolution") == 0)
                    result = convolution(&matrix1, &matrix2, 1, 4);
                else if((string(argv[count])).compare("matrix_mult")==0)
                    result = convolution(&matrix1, &matrix2, 1, 1);
                else if((string(argv[count])).compare("matrix_mult_mkl")==0)
                    result = convolution(&matrix1, &matrix2, 1, 2);
                else if((string(argv[count])).compare("matrix_mult_blas")==0)
                    result = convolution(&matrix1, &matrix2, 1, 3);
                else
                    cout << "Please Enter 'convolution'  or 'matrix_mult'";
            }
            else if((string(argv[count])).compare("no_padding")==0)
            {
                count++;
                if((string(argv[count])).compare("convolution") == 0)
                    result = convolution(&matrix1, &matrix2, 3, 4);
                else if((string(argv[count])).compare("matrix_mult")==0)
                    result = convolution(&matrix1, &matrix2, 3, 1);
                else if((string(argv[count])).compare("matrix_mult_mkl")==0)
                    result = convolution(&matrix1, &matrix2, 3, 2);
                else if((string(argv[count])).compare("matrix_mult_blas")==0)
                    result = convolution(&matrix1, &matrix2, 3, 3);
                else
                    cout << "Please Enter 'convolution'  or 'matrix_mut'";
            }
            else if((string(argv[count])).compare("custom_padding")==0)
            {
                if(argc != 7)
                {
                    cout << "Please enter a padsize\n";
                    return 0;
                }
                try{
                        padsize = std::stoi(string(argv[6]));
                    }
                    catch(std::invalid_argument){
                        cout << "Please enter valid padsize\n";
                        return 0;
                    }
                count++;
                if((string(argv[count])).compare("convolution") == 0)
                    result = convolution(&matrix1, &matrix2, 2, 4, padsize);
                else if((string(argv[count])).compare("matrix_mult")==0)
                    result = convolution(&matrix1, &matrix2, 2, 1, padsize);
                else if((string(argv[count])).compare("matrix_mult_mkl")==0)
                    result = convolution(&matrix1, &matrix2, 2, 2, padsize);
                else if((string(argv[count])).compare("matrix_mult_blas")==0)
                    result = convolution(&matrix1, &matrix2, 2, 3, padsize);
            	else
                	cout << "Please Enter 'convolution'  or 'matrix_mult'";
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
            if(argc != 6){
                cout << "Please enter all arguments: pooling filename max/avg 'poolsize' 'stride'\n";
                return 0;
            }
            Matrix matrix1 = Matrix_Read(string(argv[count++]));
            if((string(argv[count])).compare("max")==0)
            {
                    count++;
                    if(count < argc)
                    {
                        int pool_size;
                        try{
                            pool_size = std::stoi(string(argv[count]));
                        }
                        catch(std::invalid_argument){
                            cout << "Please enter valid pool_size\n";
                            return 0;
                        }
                        if(pool_size > matrix1.size() || pool_size <= 0){
                            cout << "Please enter valid pool_size (it cannot be bigger than the matrix and must be positive)\n";
                            return 0;
                        }
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
                            if(stride > matrix1.size()||stride<=0){
                                cout << "Please enter valid stride (it cannot be bigger than the matrix and must be positive)\n";
                                return 0;
                            }
                            else if(stride>pool_size){
                                cout << "Please ensure pool_size greater than or equal to stride\n";
                                return 0;
                            }
                            
                            result = Pooling(matrix1, true, pool_size, stride);
                        }
                        else
                            cout << "Please enter an integer pool_size\n";
                        
                    }
                    else
                        cout << "Please enter an integer stride\n";
                }
                else if((string(argv[count])).compare("average")==0)
                {
                    count++;
                    if(count < argc)
                    {	
                        int pool_size;
                        try{
                            pool_size = std::stoi(string(argv[count]));
                        }
                        catch(std::invalid_argument){
                            cout << "Please enter valid pool_size\n";
                            return 0;
                        }
                        if(pool_size > matrix1.size()){
                            cout << "Please enter valid pool_size (it cannot be bigger than the matrix)\n";
                            return 0;
                        }
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
                                cout << "Please enter valid stride (it cannot be bigger than the matrix)\n";
                                return 0;
                            }
                            else if(stride>pool_size){
                                cout << "Please ensure pool_size greater than or equal to stride\n";
                                return 0;
                            }
                            
                            result = Pooling(matrix1, false, pool_size, stride);
                        }
                        else
                            cout << "Please enter an integer pool_size\n";
                        
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
            
           /*for(int i = 0; i<result.size(); i++){	//Print and Return
                for(int j = 0; j<result[0].size(); j++){
                    cout << result[i][j];
                    cout << " ";
                }
                cout << "\n";
            }*/
            //cout << result[50][50];
        }
        catch(string e)
        {
            cout << "File " << e << " does not exist\n";
            return 0;
        }
        return 0;
    }
