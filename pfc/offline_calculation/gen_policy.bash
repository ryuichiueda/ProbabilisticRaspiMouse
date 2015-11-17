#!/bin/bash

dir=$(dirname $0)
sttdir=$dir/state_transition
vidir=$dir/value_iteration

rm -f $dir/policy

$sttdir/gen_state_trans > $dir/state_trans 2> $dir/final_states

echo "vi"
cat $dir/final_states	|
$vidir/value_iteration -p 10 $dir/state_trans	> $dir/values.ans

while [ ! -e $dir/policy ] ; do
	echo "vi"
	cat $dir/values.ans		|
	$vidir/value_iteration -p 10 $dir/state_trans	> $dir/values.tmp

	mv $dir/values.tmp $dir/values.ans
done

mv $dir/values.ans $dir/optimal_values
