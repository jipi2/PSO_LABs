#!/bin/bash

sudo apt-get install gparted
sudo apt-get install calibre

if ! [[ $(grep "profesori" /etc/group) ]] && if [[ $(grep "studenti" /etc/group) ]] && if [[ $(grep "FacA" /etc/group) ]] && if [[ $(grep "FacB" /etc/group) ]] && if [[ $(grep "FacC" /etc/group) ]]
then
	sudo groupadd profesori
	sudo groupadd studenti
	sudo groupadd FacA
	sudo groupadd FacB
	sudo groupadd FacC
fi


