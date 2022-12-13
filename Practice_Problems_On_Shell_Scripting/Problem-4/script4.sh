#!/bin/bash

changeExtension(){
	file=$1
	if [[ ${file##*.} = "cpp" ]]  # ${var##Pattern} Remove from $var the longest part of $Pattern that matches the front end of $var.
	then
		x=${file%.*}  # ${var%Pattern} Remove from $var the shortest part of $Pattern that matches the back end of $var.
		x+=".c"
		mv "$file" "$x"
	fi
}

traverseDirectory(){
	for file in *
	do
		if [ -d "$file" ]
		then
			(cd "$file" && traverseDirectory)
		else
			changeExtension "$file"
		fi
	done
}


traverseDirectory
