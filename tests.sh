#!/bin/bash 

mkdir tests
mkdir tests/mem

FILES=(15 1000)

for ((i=0; i < 2; i++)) do
	valgrind --log-file="tests/mem/${FILES[$i]}.mem" ./proj2 -a samples/${FILES[$i]}.b tests/${FILES[$i]}s.b
	grep -q "ERROR SUMMARY: 0 errors" tests/mem/${FILES[$i]}.mem 
	ret=$?
	if [ $ret != 0 ]; then
		echo "Memory issue for ${FILES[$i]} array"
	fi
	diff samples/${FILES[$i]}s.b tests/${FILES[$i]}s.b

	valgrind --log-file="tests/mem/${FILES[$i]}.mem" ./proj2 -l samples/${FILES[$i]}.b tests/${FILES[$i]}s.b
	grep -q "ERROR SUMMARY: 0 errors" tests/mem/${FILES[$i]}.mem
	ret=$?
	if [ $ret != 0 ]; then
		echo "Memory issue for ${FILES[$i]} list"
	fi
	diff samples/${FILES[$i]}s.b tests/${FILES[$i]}s.b
done

rm -R tests
