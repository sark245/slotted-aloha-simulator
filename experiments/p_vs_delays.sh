#!/bin/bash

# Tries different values for 'p' and collects mean delays

SIMULATOR="./aloha"
VALUES=( 0.5 0.4 0.3 0.2 0.1 0.09 0.08 0.07 0.06 0.05 0.04 0.03 0.02 0.01 0.009 0.008 0.004 0.002 0.001 0.0008 )
OUTPUT_FILE="p_vs_delays.out"


rm $OUTPUT_FILE 2> /dev/null

for p in "${VALUES[@]}"
do
	echo "Simulating with p = $p"
	delay=`$SIMULATOR $p | grep "Mean" | cut -f 2 -d ':'`
	printf "%s\t%s\n" $p $delay >> $OUTPUT_FILE
done

