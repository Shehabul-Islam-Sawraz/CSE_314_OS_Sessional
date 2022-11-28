#!/bin/bash

if [[ -e "out.txt" ]]
then
	rm "out.txt"
fi

if [[ -e "err.txt" ]]
then
	rm "err.txt"
fi

i=0

while (bash fail.sh)>>out.txt 2>>err.txt
do
	(( i+=1 ))
done

echo "It took $i runs for the script to fail"
