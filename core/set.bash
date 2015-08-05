#!/bin/bash -e

###モータ励磁の警告音を出す###
echo 100 > /dev/rtbuzzer0
###モータ励磁###
echo 1 > /dev/rtmotoren0
