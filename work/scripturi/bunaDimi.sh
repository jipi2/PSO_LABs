#!/bin/bash

echo "Introdu o ora: "

read ora

if [ $ora -lt 12 ]
then
	echo "Buna Dimi"

elif [ $ora -ge 12 -a $ora -lt 18 ]
then
	echo "Buna Ziua"

elif [ $ora -ge 18 -a $ora -le 24 ]
then
	echo "Noapte Buna"
else
	echo "Eroare"
fi
