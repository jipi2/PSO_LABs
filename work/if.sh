#!/bin/bash


declare -i sum="$2"+"$3"
declare -i prod="$2"*"$3"
 
# commentariu
if [[ -d "$1" ]]
then
    echo $sum > "$1/suma"
    echo $prod > "$1/produs"
else
    printf "error!!!"
    printf "%s %d" Dan 13
fi
