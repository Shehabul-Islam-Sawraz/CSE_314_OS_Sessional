#!/bin/bash

for file in $(ls | grep -E '([A-Za-z]*[0-9]+)+')
do
	if [ -f "$file" ]
	then
		rm "$file" && echo "File named \"$file\" deleted"	 
	fi
done
