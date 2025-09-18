# list/Makefile
#
# Makefile for list implementation and test file.
#
# <Aja Wilkes>

list: list.c main.c list.h
	gcc -Wall -Wextra -std=c11 -g list.c main.c -o list

run: list
	./list

clean:
	rm -f list *.o



