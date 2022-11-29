#!/bin/bash

if (( $# == 0 )); then
	max_score=100; max_student_id=5
elif (( $# == 1 )); then
	max_score=$1; max_student_id=5
else
	max_score=$1; max_student_id=$2
fi

numbers=(); filenames=()
echo "students_id, score" > output.csv

copyChecker(){
	for (( x=$1+1;x<=$max_student_id;x++ )) ; do
		if [ -e "out"$x".sh" ] && ( diff -w -s -q -B "out"$1".sh" "out"$x".sh" &>/dev/null); then
			numbers[$x]=-$max_score && numbers[$1]=-$max_score; rm "out"$x".sh"
		fi
	done
}

traverseFiles(){
	for (( x=1; x<=max_student_id; x++ )); do	
		folder="$1"/"180512"$x; file="$folder"/"180512"$x".sh"
		filenames[$x]=$(basename -- "$folder")
		if [ -e "$folder" ] && [ -e "$file" ]; then
			cat "$file" > "out"$x".sh"
			numbers[$x]=$max_score
		else
			numbers[$x]=0
		fi
	done
}

checkOutput(){
	(bash out$1.sh) > out.txt
	x=$(diff -w out.txt AcceptedOutput.txt | grep -c "^[<>]"); (( x=max_score-x*5 ))
	if (( x <=0 )); then
		numbers[$1]=0
	else
		numbers[$1]=$x
	fi
}

checkCopy(){
	for j in ${!numbers[@]}; do
	  	if (( ${numbers[$j]} > 0 )); then
	  		copyChecker $j
	  		if (( ${numbers[$j]} > 0 )); then
		  		checkOutput $j
		  	fi
		  	rm "out"$j".sh"
	  	fi
	done 
}


resultIntoCSV(){
	for (( x=1; x<=max_student_id; x++ )); do
		echo "${filenames[$x]}, ${numbers[$x]}" >> output.csv
	done	
	rm out.txt
}

traverseFiles Submissions
checkCopy
resultIntoCSV
