#!/bin/bash
for i in {1..40};
do
	echo $i
	./a.out data.3 $i 2000 0 | grep Gewinn=
	./tabu_lsg data.3 $i 2000 0 | grep Gewinn=
done
