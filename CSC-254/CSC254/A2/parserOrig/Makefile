# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.

# Note that rule for goal (parse) must be the first one in this file.

parse: parse.o scan.o
	gcc -g3 -o parse parse.o scan.o

parse.o: scan.h
scan.o: scan.h
