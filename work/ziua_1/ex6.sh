#!/bin/bash

echo "Introduceti un nume de utilizator: "
read nume

verif=`cat /etc/passwd | cut -f1 -d: | grep $nume`

if [[ ${#verif} -gt 0 ]]
then
	for item in *
	do
		own=`stat -c%U $item`
		
		if [[ "$own" == "$nume" ]]
		then
			sudo chmod u+x $item
			echo "intra"
		fi
	done
else
	echo "Acest utilizator nu exista"
	
fi
