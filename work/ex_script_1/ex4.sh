#!/bin/bash

if [[ -n $1 && -n $2 ]]
then

vect=`ls . | egrep "\.$1"`

for i in $vect
do
	nume=`echo $i | cut -f1 -d.`
	mv $i $nume.$2
done

else

echo "Nu exista paramterii sefule"

fi
