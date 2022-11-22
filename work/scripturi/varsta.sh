#!/bin/bash

echo 'Introdu varsta: '
read varsta

if [ $varsta -gt 18 ]
then
	echo 'Esti major'
else
	echo 'Esti minor'
fi
