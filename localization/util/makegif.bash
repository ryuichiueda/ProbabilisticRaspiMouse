#!/bin/bash -xv

tmp=/tmp/$$
rm *.ppm

self 1 2 "$1"		|
uniq			|
getlast 2 2		|
self 1		> $tmp-list

./draw_list $1 $tmp-list

convert -loop 1 -delay 100 *.ppm animation.gif

open -a safari animation.gif

rm -f $tmp-*
rm *.ppm
