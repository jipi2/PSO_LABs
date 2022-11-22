#!/bin/bash

echo "Introduceti un fisier: "
read fis

if [[ -f $fis ]]
then
	sed -r "1~2s/\b([1-9])([0-9])\2\1\b//g" $fis
	vect=`sed -n "2~2p" $fis`
	sum=0
	cont=0
	for nr in $vect
	do	
		if [[ $(echo $nr | egrep "^[1-9][0-9]*$" | egrep "8") ]]
		then
			let cont++
			let sum+=nr
			echo "intra"
		fi
	done
	res=0
	let res=sum/cont
	echo "Media este: $res"
else
	echo "Nu exista acest fisier"
fi
