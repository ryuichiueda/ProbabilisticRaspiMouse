#!/bin/bash -xv

dir="$(dirname $(readlink -f $0))"

ERROR(){
	echo 0 > /dev/rtmotoren0
	exit 1	
}

X=$1
Y=$2
T=$3

trap ERROR 2

ls /dev/rtmotor0 || sudo insmod ~/RaspberryPiMouse/src/drivers/rtmouse.ko
sudo chmod 666 /dev/rt*

echo 1 > /dev/rtmotoren0

$dir/main $dir/map $dir/offline_calculation/policy $dir/offline_calculation/optimal_values $X $Y $T

echo 0 > /dev/rtmotoren0
