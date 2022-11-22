#!/bin/bash

input=$1
#output="/home/jipi/work/ex_script_1/encryptFile"
output="encryptFile"

while IFS= read line
do
	newline=`echo $line | tr "[a-wx-z]" "[d-za-d]" | tr "[A-WX-Z]" "[D-ZA-D]"`
	echo "$newline">>$output

done < $input
