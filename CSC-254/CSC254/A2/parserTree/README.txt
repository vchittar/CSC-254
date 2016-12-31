Vishnu Chittari
Yufei "Edward" Du

Assigenment 2: Syntax Error Recovery

Folder Contains:
parse.cpp
scan.cpp
scan.hpp
README.txt
test.txt         --Test cases
output.txt		 --Sample output
Makefile
parse            --Pre-compiled binary file

In this project we were required to take a look at syntax error recovery. Syntax error recovery is simply a way by which a compiler continues to parse a program even after it encounter an instance of invalid syntax. Essentially we had to implement a recovery for an extended version of the calculator language. We first started off by simply changing the C syntax to C++ syntax. Then we began to modify the parser file. First we came up with the PREDICT sets for the language, and then the FIRST and the FOLLOW. Our first approach to the project was to essentially add the if and do check statements, which was pretty self-explanatory. We then worked our way to Wirth's error recovery mechanism and successfully implemented that. Our major hurdle for the project was the output of the relation sytnax tree for which we had many different approaches. 

First we tried the "brute-force" way where we essentially threw "()s" around till we found an answer. This worked for some cases(simple ones) but failed for the overly-complicated nested ones. 

Our second approach was to create an instance of temp variables through which we keep track of certain "stages" and then push the result to the output. The problem with this was that, if we followed this route we would have had to create temp variables for mulops, addops and more on a case by case basis. 

Our third attempt, successful attempt, was to use a binary tree. The binary tree was used to store the relations. We used a stack to keep track of what gets pushed, how it gets pushed and where it gets pushed to. The stack was used to store the order of uncompleted operators. We built our own in-built Array.contains as we could not find a suitable one for C++, like those in Java or C#. In order to avoid multiple entries we were sure to reset the "output" before we used it again. The mechanics of the tree essentially works in this way:

(literals)
1. Peek at the top of the stack
2. If that node doesnt have the left child (means its empty) the item will be its left child
3. Else the literal will be the right child
(paren)
1. Peek at the top of the stack
2. If the node doesn't have a left child, create an empty left child and push it to the stack.
3. Else we create an empty right child, pop the current node and push the right child of the original node

(Operator:mo,ao,ro)
Assign the operator as the value of the peek of the stack

The program simply exits when an un-recoverable error occurs. 

To compile: simply run the makefile (type "make" in the commandline) 
To run: ./parse < test.txt

Structure of the test file:
A))):=3+((18*(B*45))/(C/22))            --Testing basic assignment of an id with complex relation, and error recovery
read D									--Testing read
write (B+I)==(C-E)						--Testing write
if R do C:=999-(10/2) od fi				--Testing if statement and do statement
check (10+(20*5))==Y					--Testing check
$$										--End of file

Sample output: 
ERROR - token not matched.
ERROR - token not matched.
ERROR - token not matched.
OUTPUT: (:= A (+ (3 )(/ (* (18 )(* (B )(45 )))(/ (C )(22 )))))(read D)(write (== (+ (B )(I ))(- (C )(E ))))(if ( (R )) (do (:= C (- (999 )(/ (10 )(2 ))))))(check (== (+ (10 )(* (20 )(5 )))(Y )))