#!/bin/bash


(( $# != 2 )) && echo "Usage: script2.sh line_no pattern" && exit 1

for file in *.txt
do
	line_no=$(cat $file | wc -l)
	if (( line_no < $1 ))
	then
		echo "$file dowsn't have $1 lines"	
	else
		if (head -n "$1" "$file" | tail -n1 | grep -q "$2") 
		then
			echo "Pattern \"$2\" found at line no $1 of $file"
			echo "Deleting $file" 
			rm "$file"
			echo "$file deleted"
		else
			echo "Pattern \"$2\" not found at line no $1 of $file"
		fi
	fi
done