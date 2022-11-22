#!/bin/bash

input="encryptFile"
output="decryptFile"

while IFS= read line
do
        newline=`echo $line | tr 'd-za-c' 'a-wx-z' | tr 'D-ZA-C' 'A-WX-Z'`
        echo "$newline">>$output

done < $input


