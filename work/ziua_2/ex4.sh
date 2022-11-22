#!/bin/bash

#arata doar cine ruleaza scriptul

pers=`ps -u | grep "/bin/bash ./ex4.sh"`
pers=`echo $pers | cut -f1 -d" "`
echo $pers
