#!/bin/bash

declare -i res=0

var=$2
case $var in
	"+") res=$1+$3 ;;
	"-") res=$1-$3 ;;
	"*") res=$1*$3 ;;
	*) echo Unknown operation!! ;;
esac

echo $res
