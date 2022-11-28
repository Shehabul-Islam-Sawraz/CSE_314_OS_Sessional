#!/bin/bash

changeExtension(){
	file=$1
	if [[ ${file##*.} = "cpp" ]]
	then
		x=${file%.*}
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
