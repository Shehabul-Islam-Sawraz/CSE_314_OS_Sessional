#!/bin/bash

(( $# < 1 )) && echo "Usage: script1.sh filename1 filename2 ..." && exit 1

for file in "$@"
do
	if [[ ! -e $file ]]
	then 
		echo "$file doesn't exist"
	elif [[ -f $file ]]
	then 
		if [[ -x $file ]]
		then 
			echo "$file is currently executable"
			echo $(ls -all "$file")
			echo "$file's executable permission is now changing"
			$(chmod -x "$file") && echo $(ls -all "$file") && echo "$file is currently not executable"
		else
			echo "$file is currently not executable" && echo $(ls -all "$file")
		fi
			
	else
		echo "$file is not a regular file"
	fi
done
