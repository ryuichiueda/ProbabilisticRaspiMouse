#!/bin/bash -xv

dir="$(dirname $(readlink -f $0))"

ERROR (){
	echo 0 > /dev/rtmotoren0
}

trap 2 ERROR

ls /dev/rtmotor0 || sudo insmod ~/RaspberryPiMouse/src/drivers/rtmouse.ko
sudo chmod 666 /dev/rt*

echo 1 > /dev/rtmotoren0

cat $dir/input		|
$dir/main

echo 0 > /dev/rtmotoren0
