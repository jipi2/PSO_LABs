#!/bin/bash

echo "Introduceti o luna in format numeric: "
read luna

if [ $luna -ge 1 -a $luna -le 2 -o $luna -eq 12 ];
then
echo "Iarna"

elif [ $luna -ge 3 -a $luna -le 5 ]
then
echo "Primavara"

elif [ $luna -ge 6 -a $luna -le 8 ]
then
echo "Vara"

elif [ $luna -ge 9 -a $luna -le 11 ]
then
echo "Toamna"

else
echo "Eroare"
fi
