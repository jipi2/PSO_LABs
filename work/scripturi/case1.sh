#!/bin/bash

echo "Intrody nr:"
read nr
case $nr in
1) echo "nr este 1"
echo "rezolvat" ;;
2)echo "nr este 2";;
*) echo "nr este alta valoare";;
esac
