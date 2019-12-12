#!/bin/bash

g++ n2puzzle.cpp -o main
time ./main <test1.txt
echo 
time ./main <test2.txt
echo
time ./main <test3.txt