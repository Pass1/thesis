#!/bin/sh

#This script needs to be used in conjuntion with interlace.py
#It will always do a "mkdir" on the output_directory, even if it exists. On most Unix systems it is fine.


if [ $# -ne 4 ]
then
	echo "Error. $0 Needs 4 arguments."
	echo "Please use: $0 <start_frame> <end_frame> <input_directory> <output_directory>"
	exit
fi
mkdir $4
for ((i=$1 ; i <= $2 ; i++ ));
do
	echo "Doing frame $i"
	python interlace.py $3/`printf "left_frame%04d.png" $i` $3/`printf "right_frame%04d.png" $i` `printf $4/"frame%04d.png" $i`
done

exit 0
