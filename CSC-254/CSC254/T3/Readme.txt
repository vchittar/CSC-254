Vishnu Chittari
Yufei "Edward" Du

Project 3: Interpretation

Included Files:
-Interpreter.ml
- Readme.txt

To run this project:
$ ocamlc str.cma interpreter.ml
$ ./a.out

In this project we were asked to implement a complete interpreter for an extened version of the calculator grammar. It can catch the following errors:
	- Divide by zero
	- Non-numeric input
	- Use of inintialized variables
	- Unexpected end of input

We first started with the "ast_ize" functions and we matched them with the ecg and and this process was repeated for most of the 'ast_ize" functions. We were able to use the ecg to create most of these functions. The hardest part was understanding how OCaml interpreted certain statements and functions and writing our code around this logic. 

The next part was to write the interpreter for the AST. We followed the same apporoach that we used in the AST to the interpreter. The evaluation process keeps track of all the variables, and other identifiers and as well as the input and the output. It recursively goes through the AST to evaluate each statement. 

Note: On out computer, the program compiles without any warning. However, on CSUG machine, the compiler has a "Warning 52" when compiling. This does not affect the result and we have no idea why this happens. 


Sample Output:
foo: symbol not found

10 5

2 3 5 7 11 13 17 19 23 29

non-numeric input

3 divide by 0

unexpected end of input