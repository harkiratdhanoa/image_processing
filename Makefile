MKLFLAGS = -L${MKLROOT}/lib -Wl,-rpath,${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lmkl_blacs_mpich_lp64 -liomp5 -lpthread -lm -ldl

OBSFLAGS = -L/usr/local/Cellar/openblas/0.3.5/lib -Bstatic -lopenblas

all: program 

program: program.o Convolution.o NonLinear.o Pooling.o Read.o Sigmoid.o mkl_mult.o mkl_multiplication.o opencblas_multiplication.o
	g++  Convolution.o NonLinear.o Pooling.o Read.o Sigmoid.o program.o mkl_mult.o mkl_multiplication.o opencblas_multiplication.o -o program $(MKLFLAGS) $(OBSFLAGS) 

program.o: program.cpp 
	g++ -c program.cpp

Convolution.o: Convolution.cpp Convolution.h mkl_mult.o mkl_multiplication.o opencblas_multiplication.o
	g++ mkl_mult.o mkl_multiplication.o opencblas_multiplication.o -c Convolution.cpp

mkl_mult.o: mkl_mult.h mkl_mult.cpp
	g++ -c mkl_mult.cpp
mkl_multiplication.o: mkl_multiplication.cpp mkl_multiplication.h
	g++ -c mkl_multiplication.cpp  
opencblas_multiplication.o: opencblas_multiplication.cpp opencblas_multiplication.h
	g++ -I/usr/local/Cellar/openblas/0.3.5/include -c opencblas_multiplication.cpp

Pooling.o: Pooling.h Pooling.cpp
	g++ -c Pooling.cpp
Sigmoid.o: Sigmoid.cpp Sigmoid.h
	g++ -c Sigmoid.cpp
Read.o: Read.h Read.cpp
	g++ -c Read.cpp
NonLinear.o: NonLinear.cpp NonLinear.h
	g++ -c NonLinear.cpp
