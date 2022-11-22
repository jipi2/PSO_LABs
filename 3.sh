#!/bin/bash

#1
if [[ -n $1 ]]
then
while read line
do
	ip=`echo $line | cut -f2 -d">" | cut -f1 -d:`
	res=`echo $ip | egrep -o "10\.3"`
	
	if [[ $res == "10.3" ]]
	then
		echo $line
	fi
	
done < $1


#2
sed -E "s/IP ([0-9]+\.)[0-9]+\./IP \1\*./g" $1

#3, 4

while read line
do
	ip_d=`echo $line | cut -f2 -d">" | cut -f1 -d:`
	ip_s=`echo $line | cut -f3 -d" "`
	
	declare -i cont=3
	
	while [[ $cont -gt 0 ]]
	do
		ping $ip_d
		ping $ip_s
		let cont-=1
	done
	
	ip_vect=`ip address | egrep -o "[0-9]+\.[0-9]+\.+[0-9]+\.[0-9]+"`
	
	for i in $ip_vect
	do
		if [[ $i == $ip_d ]]
		then
			echo "Se gaseste si pe statie de lucru: $ip_d" 
		fi
		
		if [[ $i == $ip_s ]]
		then
			echo "Se gaseste si pe statie de lucru: $ip_s" 
		fi
	done
	
done < $1

#5
sum=0
while read line
do
	aux=`echo $line | rev`
	l=`echo $aux | cut -f1 -d" "`
	l=`echo $l | rev`
	
	let sum+=l
	
done < $1

echo "Dimensiunea pachetelor este: $sum"
fi






















