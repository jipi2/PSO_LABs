#!/bin/bash

echo "Introduceti directorul: "
read dir

dir_path=`find /home/jipi -type d -name $dir`
len=${#dir_path}

if [[ $len -eq 0 ]]
then
	echo "Nu exista acest director"
else
	find $dir_path | grep "student"
fi
