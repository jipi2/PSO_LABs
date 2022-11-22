#!/bin/bash


if [[ -n $1 ]]
then

sed -i "s#${1}:x:#${1}::#" mypasswd
nume=$1
len=${#nume}
let len-=1
stele="*******************************************************************************************************************************************************************"
sed -i "s#$nume#${nume:0:1}${stele:0:$len}#" mypasswd

else
echo "Parametru este gol"
fi
