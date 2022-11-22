#!/bin/bash

function f_fisier()
{
	echo "Introduceti un nume de fisier: "
	read fis
	if [[ -e $fis ]]
	then
		type=`stat -c%F $fis`
		size=`stat -c%s $fis`
		t_acc=`stat -c%x $fis`
		
		echo "Tipul fisierului este: $type, are: $size octeti si a fost accesat ultima oara: $t_acc"
		rights=`stat -c%a $fis`
		rights=`echo $rights | rev`
		rights=${rights:0:1}
		res=`expr $rights % 2`
		
		if [[ res -eq 1 ]]
		then
			echo "Are drept de utilizare"
		else
			echo "Nu are drept de utilizare"
		fi
	else
		echo "Acest fisier nu exista"
		return 1
	fi
}


function directoare()
{
	if [[ -d $1 && -d $2 ]]
	then
		dir2_fis=`ls $2`
		dir1_fis=`ls $1`
		
		for item in $dir1_fis
		do
			time=`stat -c%x $1/$item | cut -f1 -d-`
			if [[ $time -eq 2022 ]]
			then
				flag=0
				for i in $dir2_fis
				do
					if [[ $item -eq $i ]]
					then
						flag=1
						break
					fi
				done
				if [[ $flag -eq 0 ]]
				then
					mv $1/$item $1/$item-old
					mv $1/$item-old $2
				else
					echo "Exista deja un fisier cu un nume asemanator"
				fi
			fi
		done
		
		
	else
		echo "Nu e bine"
	fi
}

PS3="Alege o varianta sefule: "
select ITEM in "Nume de fisier" "Directoare care pe care" "exit"
do
	case $REPLY in
	1)
	f_fisier
	;;
	2)
	directoare $1 $2
	;;
	3) 
	exit 0
	;;
	esac
done
