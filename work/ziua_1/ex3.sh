#!/bin/bash

echo "Introdu un nume de fisier sefule: "
read name_fis
if [[ -f $name_fis ]]
then
	hl=`stat -c%h $name_fis`
	tm=`stat -c%y $name_fis`
	inode=`stat -c%i $name_fis`
	
	echo "$hl	$tm	$inode"
else

echo "Nu exista acest fisier sefule"
fi
