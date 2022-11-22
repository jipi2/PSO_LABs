#!/bin/bash

#1

#a
ps -eo pid,pcpu | egrep "[1-9]\.[1-9]"

#b

rm temp.txt
ps -eo pid,uname,cmd > temp.txt

cont=0

while read line
do
	if [[ cont -gt 0 ]]
	then
		proc=`echo $line | cut -f3 -d" "`
			proc_name=${proc##*/}
			
			if [[ proc_name == "nmap" ]] || [[ proc_name == "nc" ]] || [[ proc_name == "sleep" ]] || [[ $(echo $proc_name | egrep "\.sh") ]]
			then
				afis=`echo $line | cut -f1-2 -d" "`
			fi
		
	fi
	
	let cont++
	
done < temp.txt
rm temp.txt

#2

#a
line_r=`cat /etc/group | grep "root"`
line_s=`cat /etc/group | grep "sudo"`
line_a=`cat /etc/group | grep "adm"`

u_r=`echo $line_r | cut -f4 -d:`
u_r=`echo $u_r | tr "," " "`

u_s=`echo $line_s | cut -f4 -d:`
u_s=`echo $u_s | tr "," " "`

u_a=`echo $line_a | cut -f4 -d:`
u_a=`echo $u_s | tr "," " "`

echo "Utilizatorii care fac parte din root: $u_r"
echo "Utilizatorii care fac parte din sudo: $u_s"
echo "Utilizatorii care fac parte din adm: $u_a"

#3

path=`echo $PATH | tr ":" " "`

for item in $path
do
	if [[ -u $item ]]
	then
		echo "Fisierul $item are SUID setat"
	fi
done

#b

own=`stat -c%U /etc/shadow`

if [[ $own == "root" ]]
then
	echo "Ownerul este root"
else
	echo "Ownerul nu este root"
fi

perm=`stat -c%a /etc/shadow`
perm=`echo $perm | rev `

res=`echo $perm | egrep "[1-9][0-9][0-9]"`
echo $res

len=${#res}

if [[ $len -eq 0 ]]
then
	echo "Nu are nicio permisiune pentru others"
else
	echo "Are permisiuni pentru others"
fi






















