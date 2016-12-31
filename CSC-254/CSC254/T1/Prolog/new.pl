:- use_module(library(lists)).
%Yufei (Edward) Du
%Vishnu Chittari
%
%For algorithm behind code check readme.
%This is the starting function. X = input, Y = Output
genList(X,Y):-
	primeGen(X,P,1),
	genList(X,P,Y).
%Base case when input gets 0
genList(0,_,[]).
%Case that Y1 is counted
genList(X,[Y1|Y],[Y1|Z]):-
	X>0,
	X1 is X-Y1,
	genList(X1,Y,Z).
%Case that Y1 (_ here) is ignored
genList(X,[_|Y],Z):-
	X>0,
	genList(X,Y,Z).
	

//Prime Generation
primeGen(X,Y):-
	primeGen(X,Y,1).
	
primeGen(0,[],_).
primeGen(1,[],_).
primeGen(X,Y,Z):- 
	X > Z,
	Xnew is X - 1,
	primeGen(Xnew, Y1, Z),
	(prime(X) -> 
		flatten([Y1,X],Y)
	;
		Y = Y1
	).


%Algorithm: https://en.wikipedia.org/wiki/Primality_test

prime(2).
prime(3).


prime(X):-
	X > 2,
	R1 is X mod 2,
	R2 is X mod 3,
	0 \= R1,
	0 \= R2,
	\+ prime_recur(X,5).
	
prime_recur(X,Y):-
	Y2 is Y*Y,
	Y2 =< X,
	(checkprime(X,Y);
	prime_recur(X,(Y+6))).

checkprime(X,Y):-
	R1 is X mod Y,
	Y2 is Y + 2,
	R2 is X mod Y2,
	(R1 == 0;
	R2 == 0).