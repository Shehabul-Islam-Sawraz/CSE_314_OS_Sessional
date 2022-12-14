#!/bin/bash

# Check no of arguments and set max_score & max_student_id based on that
if (( $# > 2 )); then
	echo "Invalid number of arguments. Please provide at most 2 arguments. Usage: ./script.sh max_score max_student_id" && exit 1
elif (( $# == 0 )); then
	max_score=100; max_student_id=5
elif (( $# == 1 )); then
	max_score=$1; max_student_id=5
else
	max_score=$1; max_student_id=$2
fi

numbers=(); filenames=()  # Arrays for storing numbers of students and their filenames
echo "students_id, score" > output.csv

copyChecker(){
	for (( x=$1+1;x<=max_student_id;x++ )) ; do
		if [ -e "out$x.sh" ] && ( diff -w -s -q -B "out$1.sh" "out$x.sh" &>/dev/null); then  # Checking if a output file for a student exists or not and comparing
																							 # it with given AcceptedOutput file. '&>/dev/null' is basically 
																							 # nullifying all the warnings or text generated in diff command
			#(( numbers[$x] > 0 )) && (( numbers[$x]=-$max_score )); (( numbers[$1] > 0 )) && (( numbers[$1]=-$max_score )); rm "out$x.sh"  # If running copy checker first, uncomment this line
			(( numbers[x] > 0 )) && (( numbers[x]=-numbers[x] )); (( numbers[$1] > 0 )) && (( numbers[$1]=-numbers[$1] )); rm "out$x.sh"  # If running outputs check first, uncomment this line
		fi
	done
}

traverseFiles(){
	for (( x=1; x<=max_student_id; x++ )); do	
		folder="$1/180512$x"; file="$folder/180512$x.sh"
		filenames[$x]=$(basename -- "$folder")
		if [ -e "$folder" ] && [ -e "$file" ]; then  # Checks if folder in student_id format match or not! If match then whether student_id.sh exists in that folder!
			cat "$file" > "out$x.sh"
			numbers[$x]=$max_score  # If sh file exists then set default number of student to max_score
		else
			numbers[$x]=0	# If sh file doesn't exists then set default number of student to 0
		fi
	done
}

checkOutput(){
	(bash "out$1.sh") > out.txt  # Creating out.txt with the output of the passed student_id having student's shell file
	x=$(diff -w out.txt AcceptedOutput.txt | grep -c "^[<>]"); (( x=max_score-x*5 ))  # Getting no of lines in diff staring with < or >
	if (( x <=0 )); then
		numbers[$1]=0
	else
		numbers[$1]=$x
	fi
}

# numbers['key']='value' to set value
# "${numbers[@]}" to expand the values
# "${!numbers[@]}" (notice the !) to expand the keys

checkCopy(){
	for j in "${!numbers[@]}"; do  # Traversing 'numbers' array
	  	if (( ${numbers[$j]} > 0 )); then # Verifies that sh file exists
	  		#copyChecker $j  		# If running copy checker first, uncomment this line 
	  		checkOutput $j			# If running output check first, uncomment this line
	  	fi
	done
	for j in "${!numbers[@]}"; do
		#if [ -e "out$j.sh" ] && (( ${numbers[$j]} > 0 )); then			# If running copy checker first, uncomment this line
	  	if [ -e "out$j.sh" ]; then # Verifies that sh file exists		# If running output check first, uncomment this line
	  		#checkOutput $j     	# If running copy checker first, uncomment this line
	  		copyChecker $j			# If running output check first, uncomment this line and next line. Either comment both this and next line.
		  	rm "out$j.sh"
		#elif [ -e "out$j.sh" ] && (( ${numbers[$j]} < 0 )); then		# If running copy checker first, uncomment this and next line	
		#	rm "out$j.sh"
	  	fi
	done 
}


resultIntoCSV(){
	for (( x=1; x<=max_student_id; x++ )); do
		echo "${filenames[$x]}, ${numbers[$x]}" >> output.csv  # Appending filenames and numbers in csv file.
	done	
	rm out.txt
}

traverseFiles Submissions
checkCopy
resultIntoCSV
