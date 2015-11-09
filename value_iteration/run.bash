#!/bin/bash -xv

dir="$(dirname $(readlink -f $0))"

ERROR(){
	echo 0 > /dev/rtmotoren0
	exit 1	
}

trap ERROR 2

ls /dev/rtmotor0 || sudo insmod ~/RaspberryPiMouse/src/drivers/rtmouse.ko
sudo chmod 666 /dev/rt*

echo 1 > /dev/rtmotoren0

cat $dir/input | $dir/main $dir/map

echo 0 > /dev/rtmotoren0
