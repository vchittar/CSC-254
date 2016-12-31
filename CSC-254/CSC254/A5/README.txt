Yufei (Edward) Du
Vishnu Chittari

Project 5: Templates 

List of Files:

sets.cc - Contains several test cases and the main method
sets.h - Contains the functionality of the program
README - Contains information about the project and out approach 
OUTPUT - Sample Output based on the test cases we were given

How to run:
g++ sets.h sets.cc
./a.out

Special Instruction when testing range sets:
carray_range_set takes 2 arguments (lowest and highest bound)
hashed_range_set takes 1 argument (size)
bin_search_range_set takes 1 argument (size)
See sets.cc for examples and test cases.

How to run (alternative on Ubuntu):
g++ -g -std=c++11 -o sets sets.cc sets.h
./sets

The purpose of this assignment was to implement variants on a set collection type. 

We started off this project by first tackling the simple sets. We dissected the sample simple set and built our carray simple set from that. We used a boolean array to represent the table. We based our hashed simple set from our previous carray, but used int array this time for table and added another boolean array to represent if the data at given index is occupied or not. The main difference was that we had to create a certain hash function that would fulfill the requirements. For our hashed set we used the "Knuth Variant on Division" as our hash function. This formula helps us provide a prime tablesize number for the set. For collisions we used linear probing, the simplest algorithm to implement. The bin simple set was slightly harder to tackle given that its structure was different from the previous two. We tweaked around with the comp class to make it compare properly with not only int and double but also string and char*(which is deprecated according to the C++ compiler). 

The carray range and the hash range were easy to implement. We based our structure on the previous simple set range example and we worked from there. The bin range gave us the most trouble as we had to account for fringe test cases. We tackled this problem by adding helper functions in the bin_search_simple_set class to handle range insertion and removal. In those functions, we wrote cases for each type of situation when inserting and removing. 