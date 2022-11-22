#!/bin/bash

declare -i nota_md
nota_md=$RANDOM%10+1

while [[ $nota_md -lt 5 ]]
do
	echo "restanta, mai incearca!"
	nota_md=$RANDOM%10+1
done

echo "Felicitari, ai trecut cu nota $nota_md"
