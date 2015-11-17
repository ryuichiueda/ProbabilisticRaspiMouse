#!/bin/bash -evx

g++ draw_values.cc -o draw_values

rm -f tmp/*

mkdir -p tmp

cat ../optimal_values		|
./draw_values

cd tmp

ls *.pgm	|
sed 's/\.pgm//'	|
xargs -I@ convert @.pgm @.png 

rm -f *.pgm
