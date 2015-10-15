#!/bin/bash -xv

dir="$(dirname $(readlink -f $0))"

$dir/main $dir/map
