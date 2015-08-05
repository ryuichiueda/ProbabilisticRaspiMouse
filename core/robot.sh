#!/bin/sh -evx

cd /run/shm/
exec 2> ./init.log

### set the driver ###
insmod /home/pi/RaspberryPiMouse/lib/Pi?B+/`uname -r`/rtmouse.ko
sleep 1
chmod 666 /dev/rt*

### run the main script ###
chmod a+x /home/pi/ProbabilisticRaspiMouse/core/main.bash
sudo -u pi /home/pi/ProbabilisticRaspiMouse/core/main.bash > /dev/null 2> /dev/null &
