#!/bin/bash -evx

g++ draw_policy.cc -o draw_policy

rm -f tmp/*

mkdir -p tmp

cat ../policy		|
./draw_policy

cd tmp

ls *.pgm	|
sed 's/\.pgm//'	|
xargs -I@ convert @.pgm @.png 

rm -f *.pgm
