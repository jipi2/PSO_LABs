#!/bin/bash

if [[ -e $1 ]]
then

list=`sed -n '0~2p' $1`
for i in $list
do
	declare sum=0
	cf_imp=`echo $i | egrep -o '[13579]'`
	for cf in $cf_imp
	do
		let sum=sum+cf
	done
	echo "$sum"
done

else
echo "Nu exista fisierul sefule"
fi
