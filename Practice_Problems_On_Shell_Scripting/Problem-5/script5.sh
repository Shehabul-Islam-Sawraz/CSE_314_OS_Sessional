#!/bin/bash

if [[ -e "out.txt" ]]  # If out.txt exists delete it
then
	rm "out.txt"
fi

if [[ -e "err.txt" ]]  # If err.txt exists delete it
then
	rm "err.txt"
fi

i=0

while (bash fail.sh)>>out.txt 2>>err.txt  # Run fail.sh and write(>> denotes append) the output in out.txt.
										  # If any error occurs (file descriptor ‘2’ for standard error), then
										  # append that error message in err.txt 
do
	(( i+=1 ))
done

echo "It took $i runs for the script to fail"
