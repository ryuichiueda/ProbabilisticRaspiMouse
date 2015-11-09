#!/bin/bash

dir=$(dirname $0)
sttdir=$dir/state_transition
vidir=$dir/value_iteration

rm -f $dir/policy

$sttdir/gen_state_trans > $dir/state_trans 2> $dir/values.0

echo "vi"
cat $dir/values.0			|
$vidir/value_iteration -p 10 $dir/state_trans	> $dir/values.ans

while [ ! -e $dir/policy ] ; do
	echo "vi"
	cat $dir/values.ans		|
	$vidir/value_iteration -p 10 $dir/state_trans	> $dir/values.tmp

	mv $dir/values.tmp $dir/values.ans
done

rm -f $dir/values.0
