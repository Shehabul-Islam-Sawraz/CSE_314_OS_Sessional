#!/bin/bash

changeExtension(){
	file=$1
	if [[ ${file##*.} = "cpp" ]]  # ${var##Pattern} Remove from $var the longest part of $Pattern that matches the front end of $var.
	then
		x=${file%.*}  # ${var%Pattern} Remove from $var the shortest part of $Pattern that matches the back end of $var.
		x+=".c"  # Adding extension with filename
		mv "$file" "$x"
	fi
}

traverseDirectory(){
	for file in *   # For all files in current directory
	do
		if [ -d "$file" ]  # If file is a directory
		then
			(cd "$file" && traverseDirectory)  # Go to that directory and traverse that
		else
			changeExtension "$file"
		fi
	done
}

traverseDirectory
