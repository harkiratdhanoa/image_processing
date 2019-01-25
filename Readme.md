
HOW TO CALL FUNCTIONS FROM COMMAND LINE

1. Convolution:
COMMAND: 
./program convolution {matrix1.txt} {matrix2.txt} padding/no_padding/custom_padding convolution/matrix_mult ‘optional’{pad size}

Where:
1) {matrix1.txt} is the filename of the input matrix 
	* we assume square matrix, else we will take nearest perfect square of numbers
	* it should be in column major order
	* we assume only numbers in the file
2) {matrix2.txt} is the filename of the kernel matrix 
	* it must be smaller than input matrix
	* we assume square matrix, else we will take nearest perfect square of numbers
	* it should be in column major order
	* we assume only numbers in the file
3) padding choice:
	- padding: normal zero padding, which will produce an output matrix of SAME size as the input matrix
	- no_padding: does not include any padding - it will produce an output matrix of size n-f+1 
	- custom_padding: requires an extra input - an integer pad size. This will add pad size number of rows of zeroes on all sides and perform convolution

4) choice of method: 
	- convolution: will execute a normal loop based convolution
	- matrix_mult: will convert the matrices to canonical forms, perform a multiplication and reconstruct the convolved output

5) {pad_size}:
	* only active in custom_padding mode, else will be ignored.
	* must be an positive. Else we will assume it to be zero.
    * must be an integer. Else we will truncate it.

EXAMPLE: if matrix is contained in ‘input.txt’ and kernel in ‘kernel.txt’

./program convolution input.txt kernel.txt padding convolution

Will perform normal convolution, and return same sized output

./program convolution input.txt kernel.txt custom_padding matrix_mult 3

Will perform convolution by multiplication, after adding 3 rows of zero pads on all sides

*Convolution is done as per definition, using flipping*

2. Activation:
COMMAND:
./program non_linear {matrix.txt} relu/tanh

Where:
1) {matrix.txt} is the filename of the input matrix
	* we assume square matrix, else we will take nearest perfect square of numbers
	* it should be in column major order
	* we assume only numbers in the file
2) choice of activation:
	- relu: will perform a relu i.e max(0, element) element by element over the whole matrix 
	- tanh: will perform a tanh element by element over the whole matrix

EXAMPLE: if matrix is contained in ‘input.txt’ 

./program non_linear input.txt relu

Will perform rely on input.txt matrix

3. Pooling:
COMMAND:
./program pooling {matrix.txt} max/average {pool_window_size} {stride}

Where:
1) {matrix.txt} is the filename of the input matrix
	* we assume square matrix, else we will take nearest perfect square of numbers
	* it should be in column major order
	* we assume only numbers in the file
2) choice of pooling:
	- max: will perform max pooling
	- average: will perform average pooling
3) {pool_window_size}: Indicates the square window size within which each pool is done
	* Must be a positive integer
	* Must be smaller or equal to matrix size
4) {stride}: An integer that indicates the placement of the windows
	* Must be a positive integer
	* Must be smaller or equal to matrix size
	* Must be less than pool_window_size

EXAMPLE:

./program pooling input.txt max 3 2

Will perform max pooling, with window size 3 and stride 2

4. Squashing:
COMMAND
./program squash {vector.txt} sigmoid/softmax

Where: 

1) vector.txt is the filename of the input vector
2) choice of squash function:
	- sigmoid
	- softmax

EXAMPLE: if vector is in file ‘vector.txt’

./program squash vector.txt softmax

Will perform softmax element by element


***************************************************************************************************

DESIGN and FUNCTIONS:

We have used C++ vectors to define matrices. We type-defined a vector of floats as a 'row' and then type-defined a vector of rows as a 'matrix'.'

A] Read.cpp

1) Matrix_Read
Takes in a filename and outputs the matrix assuming column major order

2) Vector_Read
Takes in a filename and outputs the vector

B] Convolution.cpp

1) Image_Mult_Form
Takes in a matrix(nxn) and the filter size(f) and prepares a matrix of size ((n-f+1)*(n-f+1)) x (f*f) ready for convolution
2) Filter_Mult_Form
Takes in a filter(fxf) and outputs a matrix of size (f*f)x1
3) Matrix_Mult
Takes two matrices and performs a simple iterative matrix multiplication
4) Padded_Image
It takes in a input matrix and:

If padding is selected:
    If odd filter size: 
        Add (f-1)/2 zero rows on all sides
    If even filter size:
        Add (f-1)/2 +1 zero rows on top and left; n-f+1 on bottom and right
If custom_padding is selected and padsize if p:
    If odd filter size: 
        Add p zero rows on all sides
    If even filter size:
        Add p-1 zero rows on top and left; p on bottom and right
If no_padding is selected:
    Leave the matrix as it is
BY DEFAULT, we select padding, and custom_border_size = 0

5) convolution
Takes input(nxn) and kernel(fxf).

It calls Padded_Image to get appropriate padding.

If we select convolution:
    Performs simple looping convolution
If we select matrix_mult:
    Takes the input and kernel. Calls 1) and 2) converts into canonical forms. Calls 3) and performs multiplication. Add the values in each row to complete convolution. Finally reconstruct the matrix.
By DEFAULT multiplication is chosen.
 
C] NonLinear.cpp

1) NonLinear
Takes a matrix and performs either relu or tanh, depending on select value

Pooling.cpp
Our function looks at pool window size and stride.
It adds extra zero rows on right and bottom so as to FIT the pool window at the end.
It then moves as per the stride, placing the pool window and perform max or average pool.

D] Sigmoid.cpp

1) Sigmoid.cpp
Takes a vector and performs element by element sigmoid.

2) Softmax.cpp
Takes a vector and performs softmax

E] program.cpp

contains the main function.
It reads the command prompt inputs and does nested ifs to call appropriate functions.
IT PRINTS OUTPUTS TO TERMINAL SCREEN.
