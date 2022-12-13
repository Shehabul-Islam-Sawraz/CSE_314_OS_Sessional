#!/bin/bash

traverse() {
	for file in "$1"/*
	do
	    if [ ! -d "${file}" ] ; then
		base_dir_name=$(dirname -- "$file")
		#echo "$base_dir_name"
		player_name=$(head -n1 "$file")
		country=$(head -n2 "$file" | tail -n1)
		role=$(head -n4 "$file" | tail -n1)
		dir_path="$country/$role/"
		mkdir -p "$dir_path"
		cp "$file" "$dir_path"
		if (( $? == 0 )); then  # Checks if file already exist in such a path so that can't be copied
			rm -r "$base_dir_name"
		fi
	    else
		traverse "${file}"
	    fi
	done
}


for file in $(ls)
do
	if [ -d "${file}" ] ; then
		basefile="$file"
		traverse "$file"
		noOfFiles=$(ls "$basefile" | wc -l)
		if [ $noOfFiles -eq 0 ]; then
			rm -r "$basefile"
		fi
	fi
done
