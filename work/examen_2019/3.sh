#!/bin/bash

if [[ -n $1 ]] && [[ -n $2 ]] && [[ -n $3 ]]
then

	if ! [[ $(cat /etc/passwd | cut -f1 -d: | egrep "^$1$" ) ]]
	then
		echo "Nu exista utilizatorul $1"
		exit 1
	elif ! [[ $(cat /etc/passwd | cut -f1 -d: | egrep "^$2$") ]]
	then
		echo "Nu exista utilizatorul $2"
		exit 1

	elif ! [[ $(cat /etc/group | cut -f1 -d: | egrep "^$3$") ]]
	then
		echo "Nu exista grupul $3"
		exit 1
	fi
	
	grp=`id -ng $1`
	
	if [[ $grp == $3 ]]
	then
		echo "Grupul este oke"
	else
		echo "Grupul NU este grup principal"
		exit 1
	fi
else
echo "Paramterii nu sunt oke"
fi
