#!/bin/bash

echo "Introduceti numele unui fisier"

read fis

if [[ -f $fis ]]
then
	cont=1
	sum=0
	
	while read line
	do
		res=`expr $cont % 2`
		if [[ $res -eq 1 ]]
		then
			sed -ri "s/[1-9]+[05]//g" $fis
			sed -ri "s/[05]//g" $fis
		else
			for nr in $line
			do
				res=`expr $nr % 2`
				if [[ $res -eq 0 ]]
				then
					let sum+=nr
				fi
			done	
		fi
		let cont+=1
		
	done < $fis
	echo "Suma este: $sum"
else
	echo "Acest fisier NU exista"
fi
