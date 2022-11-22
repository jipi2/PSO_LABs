#!/bin/bash

PS4='se execua linia: ${LINENO} #'
set -x
for i in 1 2 3
do
	echo $i
done
exit
