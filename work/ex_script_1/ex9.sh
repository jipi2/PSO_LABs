#!/bin/bash

if [[ -e $1 ]]
then

while read line
do
	echo "$line"
	echo -e ""
done < $1

else
echo "Nu exista acest fisier"
fi
