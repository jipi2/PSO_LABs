#!/bin/bash
nr1=`echo -n $1|wc -m`
echo $nr1
nr2=`echo -n $2|wc -m`
echo $nr2
nrcaract=$(($nr1+$nr2))
echo $nrcaract
