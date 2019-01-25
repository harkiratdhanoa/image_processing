#!/bin/sh
for i in 2 4 8 16 32 64 128 256 512 1024
do
	for j in 3 5 9 18
	do
		paste "$i""_""$j""_1".dat "$i""_""$j""_2".dat "$i""_""$j""_3".dat "$i""_""$j""_4".dat > $i_$j.dat
		echo "Looping ... number $i $j" 
	done
done