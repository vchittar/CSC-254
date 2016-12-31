# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.

# Note that rule for goal (parse) must be the first one in this file.
parse: parse.cpp scan.cpp
	g++ -o parse parse.cpp scan.cpp

parse.cpp: scan.hpp
scan.cpp: scan.hpp
