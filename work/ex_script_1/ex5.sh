#!/bin/bash
echo "introduceti un cuvant"
read cuvant
echo "cuvantul introdus este $cuvant"
echo "introduceti 2 cuvinte"
read cuvant1 cuvant2
echo "cuvintele introduse sunt $cuvant1 ......$cuvant2"
echo "introduceti un vector de elemente"
read -a elemente
echo "valoarea celui de-al doilea element din vector este ${elemente[1]}"
