#!/bin/bash

nume=`echo $1 | egrep -o "\/[a-zA-Z\.]+$" | egrep -o "[a-zA-Z\.]+"`
echo "Nume fisier: $nume"

len=${#nume}
echo "Lungime fisier: $len"

den=`echo $nume | cut -d\. -f1`
echo "Nume fisier: $den"

ext=`echo $nume | cut -d\. -f2`
echo "Extensie fisier: $ext"
