#!/bin/bash

for item in /home/jipi/work/ziua_1/dir_test/*
do
	echo "${item##*/}"
	echo "Pentru stergere apasati tasta 1"
	echo "Pentru arhivare apasati tasta 2"
	
	echo "Tasta: "
	
	read tasta
	
	case $tasta in
	
	1) 
	rm $item ;;
	2)
	arhvName=`echo ${item##*/} | cut -f1 -d\.`
	tar -Pcf /home/jipi/work/ziua_1/dir_test/$ahrvName.tar "$item"
        ;;
	esac
done
