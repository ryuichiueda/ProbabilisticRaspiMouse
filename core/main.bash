#!/bin/bash -xv

exec &> /run.bash/shm/main.bash.log
core=$(dirname $0)

while true ; do
	###init###
        $core/init.bash
	#準備完了の合図
	echo 1000 > /dev/rtbuzzer0
	sleep 0.2
	echo 0 > /dev/rtbuzzer0
	echo 1 > /dev/rtled0
	$core/wait_sw_push 0
	$core/wait_sw_unpush 0

	###set###
        $core/set.bash
	#準備完了の合図
	echo 1 > /dev/rtled1
	$core/wait_sw_push 0
	$core/wait_sw_unpush 0

	###run.bash###
	#run.bash起動の合図
	echo 0 > /dev/rtbuzzer0
	echo 1 > /dev/rtled2
        $core/../run.bash &

	###wait the finish button###
	$core/wait_sw_push 0
	$core/wait_sw_unpush 0
        killall -1 run.bash
done
