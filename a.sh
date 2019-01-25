#!/bin/sh
for i in 512 1024
do
	for j in 3 5 9 18
	do
		./gen_mat $i
		./fil_mat $j
		for k in 1 2 3 4 5
		do
			./program convolution matrix.txt kernel.txt padding matrix_mult >> data/"$i""_""$j""_1".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_mkl >> data/"$i""_""$j""_2".dat
			./program convolution matrix.txt kernel.txt padding convolution >> data/"$i""_""$j""_4".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_blas >> data/"$i""_""$j""_3".dat
		done
		./gen_mat $i
		./fil_mat $j
		for k in 1 2 3 4 5
		do
			./program convolution matrix.txt kernel.txt padding matrix_mult >> data/"$i""_""$j""_1".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_mkl >> data/"$i""_""$j""_2".dat
			./program convolution matrix.txt kernel.txt padding convolution >> data/"$i""_""$j""_4".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_blas >> data/"$i""_""$j""_3".dat
		done
		./gen_mat $i
		./fil_mat $j
		for k in 1 2 3 4 5
		do
			./program convolution matrix.txt kernel.txt padding matrix_mult >> data/"$i""_""$j""_1".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_mkl >> data/"$i""_""$j""_2".dat
			./program convolution matrix.txt kernel.txt padding convolution >> data/"$i""_""$j""_4".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_blas >> data/"$i""_""$j""_3".dat
		done
		./gen_mat $i
		./fil_mat $j
		for k in 1 2 3 4 5
		do
			./program convolution matrix.txt kernel.txt padding matrix_mult >> data/"$i""_""$j""_1".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_mkl >> data/"$i""_""$j""_2".dat
			./program convolution matrix.txt kernel.txt padding convolution >> data/"$i""_""$j""_4".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_blas >> data/"$i""_""$j""_3".dat
		done
		./gen_mat $i
		./fil_mat $j
		for k in 1 2 3 4 5
		do
			./program convolution matrix.txt kernel.txt padding matrix_mult >> data/"$i""_""$j""_1".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_mkl >> data/"$i""_""$j""_2".dat
			./program convolution matrix.txt kernel.txt padding convolution >> data/"$i""_""$j""_4".dat
			./program convolution matrix.txt kernel.txt padding matrix_mult_blas >> data/"$i""_""$j""_3".dat
		done
		echo "Looping ... number $i $j" 
	done
done