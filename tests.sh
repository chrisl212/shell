#!/bin/bash 

mkdir tests
mkdir tests/mem

FILES=(15 1000 10000 100000 1000000)

for ((i=0; i < 5; i++)) do
	printf "${FILES[$i]}\n==========\n"
	printf "a\n"
	valgrind --log-file="tests/mem/${FILES[$i]}.mem" ./proj2 -a samples/${FILES[$i]}.b tests/${FILES[$i]}s.b
	grep -q "ERROR SUMMARY: 0 errors" tests/mem/${FILES[$i]}.mem 
	ret=$?
	if [ $ret != 0 ]; then
		echo "Memory issue for ${FILES[$i]} array"
	fi
	diff samples/${FILES[$i]}s.b tests/${FILES[$i]}s.b
	ret=$?
	if [ $ret != 0 ]; then
		echo "Sorting incorrect for ${FILES[$i]} array"
	fi

	printf "\nl\n"
	valgrind --log-file="tests/mem/${FILES[$i]}.mem" ./proj2 -l samples/${FILES[$i]}.b tests/${FILES[$i]}s.b
	grep -q "ERROR SUMMARY: 0 errors" tests/mem/${FILES[$i]}.mem
	ret=$?
	if [ $ret != 0 ]; then
		echo "Memory issue for ${FILES[$i]} list"
	fi
	diff samples/${FILES[$i]}s.b tests/${FILES[$i]}s.b
	ret=$?
	if [ $ret != 0 ]; then
		echo "Sorting incorrect for ${FILES[$i]} array"
	fi
	printf "\n\n"
done

rm -R tests
