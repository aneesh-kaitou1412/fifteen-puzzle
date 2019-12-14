#!/bin/bash

g++ fifteenPuzzle.cpp -o main
for i in test/test*.txt; do
	time ./main <"$i" >"output/out${i##test/test}"
	echo 
	sleep 2
	python visualize.py --gridfile "$i" --stepfile "output/out${i##test/test}"
done

# time ./main <test1.txt
# echo
# time ./main <test2.txt
# echo
# time ./main <test3.txt
# echo
# time ./main <test4.txt
# echo
# time ./main <test5.txt
# echo