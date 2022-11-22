#!/bin/bash

time1="0"
time3="0"

for i in {1..20} 
do
	time1="$time1 + $(./brighten|tail -n1|cut -f2 -d' ')"
	time3="$time3 + $(./brighten_sse|tail -n1|cut -f2 -d' ')"
done

time1=`echo $time1 | bc`
time3=`echo $time3 | bc`
time1=`echo "scale=7;$time1 / 10"|bc`
time3=`echo "scale=7;$time3 / 10"|bc`

echo -e "Time without MMX =\t 0$time1"
echo -e "Time with SSE2 =\t\t 0$time3"

