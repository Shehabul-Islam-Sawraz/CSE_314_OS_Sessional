#!/bin/bash

for file in *; do  # Runnin code for all files in current directory
	if [ "$file" = "script3.sh" ]; then  # If file is our script file then ignore
		continue
	elif [ -f "$file" ]; then  # If file is a normal file(not a directory)
		filename=${file%.*}  # Fetch the filename
		if [[ "$filename" =~ [0-9] ]]; then  # Checking if filename contains number
			rm "$file" && echo "File named \"$file\" deleted"  # Removing the file
		fi
	fi
done

: "
	1. Check if variable VAR contains numbers:  
		$VAR =~ [0-9]
	2. Check if variable VAR contains only numbers:
		$VAR =~ ^[0-9]+$
	3. Check if variable VAR contains only alphabets and numbers:
		$VAR =~ ^[0-9a-zA-Z]+$
	4. Check if variable VAR starts with character 'A':
		$VAR =~ ^[A]
	5. Check if variable VAR contains special characters
		$VAR == *['!'@#\$%^\&*()_+]*
	6. Check if variable VAR contains vowels:
		$VAR =~ [AEIOUaeiou]
"