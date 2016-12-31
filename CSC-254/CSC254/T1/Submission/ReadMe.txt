Yufei "Edward" Du
Vishnu Chittari

CSC254 Project 1

List of files:
new.pl                   --Prolog Source File
ocaml.ml                 --OCaml Source File
Program.cs               --C# Source File
python.py                --Python Source File
RandomPrimeChecker.java  --Java Source File
Output.txt               --Sample Output
ReadMe.txt               --This Document

Compile / Run Instructions
C#: mcs Program.cs
mono Program.exe 39(input) OR mono Program.exe (The program will ask you to input a number)

Python: python python.py
The program will ask for input

Prolog: swipl new.pl
genList(Input, Output).

OCaml: ocamlc -o name ocaml.ml
No Test Method (Generating a list of prime partitions of 2 numbers work)

Ada: gnatmake primes.adb
No Test Method (Prime checker works)

Java: javac RandomPrimeCheck.java
java RandomPrimeChecker

We came up with a naive algorithm and wrote a Java program (Also included) testing it. After that, we tried to port this algorithm to all 5 languages. It worked well on C# and Python, but on Prolog this algorithm is just way too messy to implement, and for Ocaml and Ada we did not have enough time to finish coding. After spending a day trying to port the algorithm for Prolog, we decided to come up with a new algorithm just for Prolog using Prolog's nice "redo" feature instead of using the old one. 

Here is the algorithm for C# and Python:
Generate a list of prime numbers that are smaller than input -> 
Generate the list of two number partitions of the input ->
Extend the list with the list of three number partitions of the input ->
Go through the list of partitions and expand each number to their partitions and get rid of duplicates ->
return the list

To generate the list of three number partitions, we used the algorithm below:
Take the list of prime numbers, from i = 0 to the length of the prime list:
Generate the list of two number partitions of (input - primes[i])->
Extend each result from that list with primes[i], and get rid of the duplicates->
Add this list to the result list

As you can see the runtime is relatively harsh. Inputs more than 90 would need really long time to compute.

Here is the algorithm for Prolog:
Generate a list of prime numbers that are smaller than input ->
A recursive function: If input is less than 0 already, return false; else, pop the prime list, either ignore it and recurse or add it to the output, subtract it from the input, and recurse with the new input and output ->
Keep recursing until it hits the base case: when input is 0, return whatever output passed in and go back up. 

This algorithm is able to find all solutions because Prolog compiler will automatically retry all possible approaches. with the either-or part in the recursive function, Prolog will try each combination of the numbers from the list and return all answers that fulfill the requirement. 

Progress for OCaml and Ada
Ocaml: Able to check 2 prime number partitions of given number, able to generate prime numbers and check prime numbers. It does not have a working test method. Right now it is printing the results for number 100. Cite: http://stackoverflow.com/questions/9134929/print-a-list-in-ocaml for print_list function.
Ada: Able to check if a number is prime. 


In general, I think it is a really tough project for the first project of this course. I (we) underestimated the amount of work and did not start early enough to finish this. 