all:
	gcc -std=c99 -g -Wall -Wshadow -Wvla -pedantic src/*.c -o proj2
gen:
	gcc util/nums.c -o gen
