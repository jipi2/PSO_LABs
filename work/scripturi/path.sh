#!/bin/bash
#Acest script afiseaza fiecare director din $PATH pe cate un rand
#IFS=:
for DIR in $PATH
do
echo $DIR
done
