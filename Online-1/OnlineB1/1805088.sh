#!/bin/bash

indentation(){
	str=""
	for ((x=0;x<$1;x++)); do
		str+="  "
	done
	str+="|--"
	echo "$str$2"
}

traverseDirectory(){
	for file in *
	do
		x=$(ls | wc -l)
		if [ $x -eq 0 ]; then
			continue
		fi
		basefile="$file"
		if [ "$file" = "1805088.sh" ]; then
			continue
		elif [[ ${file##*.} = "zip" ]]; then
			basefile=${file%.*}
			unzip -q "$file"
			rm "$file"
			indentation $1 "$basefile"
			(( space=$1+1 ))
			(cd "$basefile" && traverseDirectory $space)
		elif [ -d "$file" ]; then
			indentation $1 "$file"
			(( space=$1+1 ))
			(cd "$file"  && traverseDirectory $space)
		else
			indentation $1 "$file"
		fi
	done	
}

echo "sample"
cd sample/
traverseDirectory 0

### N.B: In the ques you are said to print the tree for current directory. 
# For this, just copy this(1805088.sh) file in the 'sample' folder and 
# comment the line no 40.