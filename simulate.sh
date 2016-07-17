#!/bin/bash
FEQ=1;
MAX=15;
while [[ $FEQ -lt $MAX ]]; do
	./waf --run "scratch/hd-scenario --sendProbility=$FEQ" >result/"no-$FEQ%.txt";
	let FEQ=$FEQ+1;
	echo $FEQ;
done