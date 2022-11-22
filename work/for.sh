#!/bin/bash

lista_studenti="baluti barbusi jipianu surdu toma"

declare -i nota
for student in $lista_studenti
do 
	nota=$RANDOM%10+1
	echo "$student are nota $nota"
done
