#!/bin/bash

vect=`ls .`

for i in $vect
do
	if [[ -f $i && -u $i ]]
	then
		owner=`stat -c %U $i`
		if [[ $owner = 'jipi' ]]
		then
		
			declare -i nrLit=`echo $i | wc -m`
			let nrLit=nrLit-1
			if [[ $nrLit = 5 ]]
			then
				echo "$i"
			fi
		fi
	fi
done
