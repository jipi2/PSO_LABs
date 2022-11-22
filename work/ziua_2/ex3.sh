#!/bin/bash

echo "Introduceti un nume de fisier: "
read fis
if [[ -f $fis ]]
then
	sed -E "2~2 s/[0-9]?+[05]//g" $fis
	vect=`sed -n "1~2p" $fis`
	sum=0
	cont=0
	for nr in $vect
	do
		let cont++
		let sum+=nr
	done
	res=0
	let res=sum/cont
	echo "Media aritmetica este: $res"
else
	echo "nu exista"
fi	
