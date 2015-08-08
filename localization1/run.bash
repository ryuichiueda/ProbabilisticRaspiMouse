#!/bin/bash -xv

dir="$(dirname $(readlink -f $0))"

cat $dir/input		|
$dir/main $dir/map
