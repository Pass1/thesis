#!/bin/sh

if [ $# -ne 3 ]
then
        echo "Error. $0 Needs 3 arguments."
        echo "Please use: $0 <input_directory> <x_tiles> <y_tiles>"
        exit
fi

i=0
while [[ -e $1/`printf "frame%04d.png" $i` ]]; do
        echo "Doing $i"
        echo "python tiling.py $1/`printf "frame%04d.png " $i` $2 $3"
        python tiling.py $1/`printf "frame%04d.png " $i` $2 $3
        let i++
done
