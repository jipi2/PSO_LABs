#!/bin/bash

if [[ -d $1 ]]
then
	for item in $1/*
	do
		if [[ -d $item ]]
		then
			aux=`echo $item`
			rm -r $item
			touch $aux.txt
		fi
	done
	
	for item in $1/*
	do
		ls $1 > $item
	done
else
	echo "Directorul nu exista sefule"
fi
