#!/bin/bash
#script care afiseaza numerele intre 0 si 100
i=0
while [ $i -le 100 ]
do
    echo "i=$i"
    let i=i+1	#echivalent cu i=$(($i+1))     sau        i=$[$i+1]
    #let "i=i+1" merge si asa
done

#let -> comanda inclusa in bash pentru evaluarea expresiilor aritmetice
