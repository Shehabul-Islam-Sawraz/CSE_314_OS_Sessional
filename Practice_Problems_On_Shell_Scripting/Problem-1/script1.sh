#!/bin/bash

(( $# < 1 )) && echo "Usage: script1.sh filename1 filename2 ..." && exit 1  # If no of arguments is < 1, then show usage and exit

for file in "$@"  # Running code for all arguments
do
	if [[ ! -e $file ]]  # Checking if file exists or not!
	then 
		echo "$file doesn't exist"
	elif [[ -f $file ]]  # Checking if the file is a regular file or not!
	then 
		if [[ -x $file ]] # Checking if the file is executable
		then 
			echo "$file is currently executable"
			echo $(ls -all "$file")  # Showing the permission info of the file
			echo "$file's executable permission is now changing"
			$(chmod -x "$file") && echo $(ls -all "$file") && echo "$file is currently not executable"  # Taking back executable permission
		else
			echo "$file is currently not executable" && echo $(ls -all "$file")
		fi	
	else
		echo "$file is not a regular file"
	fi
done
