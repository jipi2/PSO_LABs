#!/bin/bash

echo "Introduceti numele utilizatorului: "
read nameUt

echo "Introduceti un fisier executabil: "
read nameFis

if ! [[ -x $nameFis ]]
then
	echo "Nu este un fisier executabil"
	exit 0
fi

procese=`ps -u $nameUt -o pid`
for i in $procese
do
	echo $i
done

for i in $procese
do
	if [[ $i -gt 10000 ]]
	then
		kill -9 $i
	fi
done
