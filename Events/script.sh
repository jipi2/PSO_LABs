#!/bin/bash
python3 EventsPython.py | egrep -o "event[0-9+]" > temp.txt
eventName=`head -n1 temp.txt`
rm temp.txt
gcc readEvents.c -o readEvents
echo "$eventName" > eventNumber.txt
rm store.txt 2> /dev/null
touch store.txt
./readEvents eventNumber.txt &
