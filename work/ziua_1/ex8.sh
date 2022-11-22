#!/bin/bash

function getType()
{
	
	res=`echo $1 | grep "[a-zA-Z]"`
	len=${#res}

	if [[ $len -gt 0 ]]
	then
		return 2
	fi
	
	res=`echo $1 | grep "\."`
	len=${#res}
	
	if [[ $len -gt 0 ]]
	then
		return 1
	fi
	
	return 0

}

if [[ -n $1 ]]
then

getType $1
echo $?

else
echo "Argumentul nu este bun"
fi
