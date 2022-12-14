#!/bin/bash


(( $# != 2 )) && echo "Usage: script2.sh line_no pattern" && exit 1  # If no of arguments is != 2, then show usage and exit

for file in *.txt  # Run loop for all text files
do
	line_no=$(cat "$file" | wc -l)  # Count total line no of the file
	if (( line_no < $1 ))  # Checking if out given line_no is greater than the total line no
	then
		echo "$file dowsn't have $1 lines"	
	else
		if (head -n "$1" "$file" | tail -n1 | grep -q "$2")   # Checking if pattern exists in given line_no
		then
			echo "Pattern \"$2\" found at line no $1 of $file"
			echo "Deleting $file" 
			rm "$file"		# Removing file
			echo "$file deleted"
		else
			echo "Pattern \"$2\" not found at line no $1 of $file"
		fi
	fi
done