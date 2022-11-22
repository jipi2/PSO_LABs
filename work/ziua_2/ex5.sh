#!/bin/bash

res=`sudo dd if=/dev/sda skip=32 bs=512 count=1 | xxd` 
echo $res
