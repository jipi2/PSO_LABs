#!/bin/bash

PS3="Alege o optiune din meniu sefule: "
select item in "Afiseaza detalii despre un anumit fisier" "Numar caractere director" "Exit"
do

	case $REPLY in
	1) 
		if [[ -e $1 ]]
		then
			mask=`stat -c%a $1`
			own=`stat -c%U $1`
			name=${1##*/}
			
			echo "$name	$own	$mask" >> output.txt
		else
		echo "Nu exista acest fisier"
		fi
		;;
	2)	
		echo "Introduceti directorul cautat: "
		read dir
		path=`find /home/jipi -type d -name $dir`
		len=${#path}

		if [[ $len -eq 0 ]]
		then
			echo "Nu exista acest director"
		else
			declare -i sum=0
			
			for item in $path/*
			do      
				if [[ -f $item ]]
				then
					name_item=${item##*/}
					let sum+=${#name_item}
					echo $name_item
				fi	
			done
			
		echo "Numar caractere: $sum"
		fi
		
		;;
	3)
		exit 0;;
	esac
done
