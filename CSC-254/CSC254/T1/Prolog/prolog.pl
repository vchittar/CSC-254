:- use_module(library(lists)).
:- dynamic adduplist/2.

%prime_partitions(input, output)
prime_partitions(0,[]).
prime_partitions(1,[]).
prime_partitions(2,[2]).
prime_partitions(17,[2,3,5,7]).
prime_partitions(X):-
	prime_partitions_rec(X),
	primeGen(X,P),
	partitions_loop1(X,P),
	adduplist(Xp,X),
	partitions_loop2(Xp,[],X).
	
%partitions_loop1(input, list of primes to input)
partitions_loop1(_,[]).
partitions_loop1(X,[Y1|Y]):-
	X1 is X-Y1,
	prime_partitions_rec(X1,Y1,Y).
	
%partitions_loop2(each subset of input until now, list of numbers in this subset before current number, Original input)
partitions_loop2([],_,_).
partitions_loop2([X1|X],Z,Xo):-
	flatten([Z,X1],Z1),
	partitions_loop2(X, Z1, Xo),
	prime_partitions(X1),
	adduplist(Y,X),
	flatten([Z,Y,X],Y1),
	sort(Y1,Y2),
	(adduplist(Y2,Xo) ->
		true
	;
		assert(adduplist(Y2,Xo))).
	
	

%prime_partitions_rec(input)
prime_partitions_rec(0).
prime_partitions_rec(1).
prime_partitions_rec(2).
prime_partitions_rec(X):-
	primeGen(X,P),
	partitions_rec_loop1(P,X).

%prime_partitions_rec(input, number to add to the head of the output, minimum prime number-1)
prime_partitions_rec(X,Y1,Y):-
	partitions_rec_loop1(Y,X,Y1).
	
%partitions_rec_loop1([CurrentNumber | RestPrimeNumberList], input)
partitions_rec_loop1([],_).
partitions_rec_loop1([I|P],X):-
	partitions_rec_loop2(I,P,X,Y1),
	(Y1 == [] ->
		partitions_rec_loop1(P,X)
	;
		(adduplist(Y1,X) -> 
			true
		;
			assert(adduplist(Y1,X))),
		partitions_rec_loop1(P,X)).
		
partitions_rec_loop1([],_,_).
partitions_rec_loop1([I|P],X,Y):-
	partitions_rec_loop2(I,P,X,Y1),
	(Y1 == [] ->
		partitions_rec_loop1(P,X,Y)
	;
		X1 is X + Y,
		flatten([Y,Y1],Z),
		(adduplist(Z,X1) -> 
			true
		;
			assert(adduplist(Z,X1))),
		partitions_rec_loop1(P,X,Y)).
	
%partitions_rec_loop2(LeftTracker, [RightTracker | RestPrimeNumberList], input, output)
partitions_rec_loop2(_,[],_,[]).
partitions_rec_loop2(I1,[I2|P], X, Y):-
	IT is I1+I2,
	((X == IT) ->
		Y = [I1,I2]
	;
		partitions_rec_loop2(I1,P,X,Y)
		).















%Support methods: primeGen = generate prime numbers; prime = check if prime

%adduplist([2],2).


%Cite: http://stackoverflow.com/questions/9875760/sum-of-elements-in-list-in-prolog
addSum([X],X).
addSum([X1,X2|X],Y):-
	addSum([X1+X2|X],Y).


unnested(_,[]).
unnested([I|[X]],[I|Y]):-
	unnested(X,Y).
	
	

primeGen(X,Y):-
	primeGen(X,Y,1).
	
primeGen(0,[],_).
primeGen(1,[],_).
primeGen(2,[2],_).
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
	X > 1,
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