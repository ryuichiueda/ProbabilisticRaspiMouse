#!/bin/bash -e

###LEDを消す###
echo 0 | tee /dev/rtled?

###モータを止める###
echo 0 | tee /dev/rtmotor[_e]*

