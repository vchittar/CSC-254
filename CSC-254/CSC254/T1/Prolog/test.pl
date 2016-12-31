/*CSC 173 Prolog Week 3-4
*Yufei (Edward) Du
*Last update: 11/5/2015
*/

/*Code from the section 5.3 of the textbook, Programming in Prolog by Clocksin & Mellish.*/
read_in([W|Ws]) :- get_char(C),
                   readword(C,W,C1),
                   restsent(W,C1,Ws).

restsent(W, _, []) :- lastword(W),!.
restsent(_, C, [W1|Ws]) :- readword(C,W1,C1),
	                   restsent(W1,C1,Ws).

readword(C,C,C1) :- single_character(C),!,get_char(C1).
readword(C,W,C2) :-
	in_word(C, NewC),
	!,
	get_char(C1),
	restword(C1,Cs,C2),
	atom_chars(W,[NewC|Cs]).
readword(_,W,C2):-get_char(C1),readword(C1,W,C2). % skipping blanks.

restword(C,[NewC|Cs],C2) :-
	in_word(C,NewC),
	!,
	get_char(C1),restword(C1,Cs,C2).
restword(C,[],C).

in_word(C,C) :- letter(C,_).
in_word(C,L) :- letter(L,C).
in_word(C,C) :- digit(C).
in_word(C,C) :- special_character(C).

special_character('-').
special_character('''').

single_character(',').          single_character(':').
single_character('.').          single_character('?').
single_character(';').          single_character('!').

letter(a,'A').		  letter(n,'N').
letter(b,'B').		  letter(o,'O').
letter(c,'C').		  letter(p,'P').
letter(d,'D').		  letter(q,'Q').
letter(e,'E').		  letter(r,'R').
letter(f,'F').		  letter(s,'S').
letter(g,'G').		  letter(t,'T').
letter(h,'H').		  letter(u,'U').
letter(i,'I').		  letter(v,'V').
letter(j,'J').		  letter(w,'W').
letter(k,'K').		  letter(x,'X').
letter(l,'L').		  letter(y,'Y').
letter(m,'M').		  letter(z,'Z').

digit('0').        digit('5').
digit('1').        digit('6').
digit('2').        digit('7').
digit('3').        digit('8').
digit('4').        digit('9').

lastword('.').
lastword('!').
lastword('?').
/*End of textbook's code*/


/*Facts
*Syntax:
*noun(NameShown, Input, RemainingInput, singular/plural, vowel)
*determiner(NameShown, Input, RemainingInput, singular/plural, forvowel)
*verb(NameShown, Input, RemainingInput, singular/plural)
*trans(x): This verb x should have a noun phrase after it
*beverb(NameShown, Input, RemainingInput, singular/plural)
*adj(NameShown, Input, RemainingInput)
*rel(NameShown,Input,RemainingInput,Object/Subject)
*sentence_finisher(Input, RemainingInput)
*/

noun(noun(apple),[apple|S],S,singular,true).
noun(noun(boy),[boy|S],S,singular,false).
noun(noun(girl),[girl|S],S,singular,false).
noun(noun(government),[government|S],S,singular,false).
noun(noun(watermelon),[watermelon|S],S,singular,false).
noun(noun(person),[person|S],S,singular,false).
noun(noun(apples),[apples|S],S,plural,true).
noun(noun(boys),[boys|S],S,plural,false).
noun(noun(girls),[girls|S],S,plural,false).
noun(noun(governments),[governments|S],S,plural,false).
noun(noun(watermelons),[watermelons|S],S,plural,false).
noun(noun(people),[people|S],S,plural,false).
determiner(determiner(a),[a|S],S,singular,false).
determiner(determiner(an),[an|S],S,singular,true).
determiner(determiner(the),[the|S],S,singular,false).
determiner(determiner(the),[the|S],S,plural,false).
determiner(determiner(the),[the|S],S,singular,true).
determiner(determiner(the),[the|S],S,plural,true).
determiner(determiner(some),[some|S],S,singular,false).
determiner(determiner(some),[some|S],S,plural,false).
determiner(determiner(some),[some|S],S,singular,true).
determiner(determiner(some),[some|S],S,plural,true).
determiner(determiner(all),[all|S],S,plural,true).
determiner(determiner(all),[all|S],S,plural,false).
verb(verb(conscript),[conscript|S],S,plural).
verb(verb(like),[like|S],S,plural).
verb(verb(run),[run|S],S,plural).
verb(verb(conscripts),[conscripts|S],S,singular).
verb(verb(likes),[likes|S],S,singular).
verb(verb(runs),[runs|S],S,singular).
trans(conscript).
trans(conscripts).
trans(like).
trans(likes).
intrans(run).
intrans(runs).
beverb(verb(is),[is|S],S,singular).
beverb(verb(are),[are|S],S,plural).
adj(adjective(evil),[evil|S],S).
rel(relative(that),[that|S],S,subject).
rel(relative(that),[that|S],S,object).
rel(relative(whom),[whom|S],S,object).
rel(relative(who),[who|S],S,subject).
rel(relative(which),[which|S],S,subject).
rel(relative(which),[which|S],S,object).
sentence_finisher([.|S],S).
sentence_finisher([!|S],S).
sentence_finisher([?|S],S).



/*Parser
*Variable NP stands for Noun Phrase
*Variable VP stands for Verb Phrase
*Variable N stands for Noun word
*Variable D stands for Determiner
*Variable vowel stands for boolean vowel
*Variable SP stands for Singular or Plural
*Variable V stands for Verb word
*Variable R stands for relative clause
*/

/*S0 stands for the list of input word
*S stands for expected remaining list
*/
sentence(sentence(NP,VP),S0,S):-
	noun_phrase(NP,S0,S1,SP),
	verb_phrase(VP,S1,S2,SP),
	sentence_finisher(S2,S).
noun_phrase(noun_phrase(D,N),S0,S,SP):-
	determiner(D,S0,S1,SP,Vowel),
	noun(N,S1,S,SP,Vowel).
noun_phrase(noun_phrase(N),S0,S,SP):-
	noun(N,S0,S,SP,_).
noun_phrase(noun_phrase(D,N,RC),S0,S,SP):-
	determiner(D,S0,S1,SP,Vowel),
	noun(N,S1,S2,SP,Vowel),
	rel_clause(RC,S2,S,SP).
verb_phrase(verb_phrase(V),S0,S,SP):-
	verb(V,S0,S,SP).
verb_phrase(verb_phrase(V,NP),S0,S,SP):-
	verb(V,S0,S1,SP),
	noun_phrase(NP,S1,S,_).
verb_phrase(verb_phrase(V,A),S0,S,SP):-
	beverb(V,S0,S1,SP),
	adj(A,S1,S).
verb_phrase(verb_phrase(V,NP),S0,S,SP):-
	beverb(V,S0,S1,SP),
	noun_phrase(NP,S1,S,_).
rel_clause(rel_clause(R,VP),S0,S,SP):-
	rel(R,S0,S1,subject),
	verb_phrase(VP,S1,S,SP).
rel_clause(rel_clause(R,NP,V),S0,S,_):-
	rel(R,S0,S1,object),
	noun_phrase(NP,S1,S2,SP),
	verb(V,S2,S,SP).
/*parse is a combination of reading and parsing*/
parse(X):-
	read_in(Y),
	sentence(X,Y,[]).
	
/*Translator*/

/*Define custom operators*/
?-op(500,xfy,&).
?-op(600,xfy,->).

/*P is the output, sentence(NP,VP) is input. 
*Most variables have same meaning with the parser except for the following
*Variable X is the current variable (E.g. x1, x2, etc.)
*Variable DET is the determiner
*Variable RC is the relative clause
*Variable ADV is the adjective
*/
ss(P,sentence(NP,VP)):-
	np(X,P1,P,NP),
	vp(X,P1,VP).
np(X,P1,P,noun_phrase(DET,N)):-
	gensym(x,X),
	det(X,P1,P2,P,DET),
	n(X,P2,N).
np(X,P1,P,noun_phrase(DET,N,RC)):-
	gensym(x,X),
	det(X,P1,P2,P,DET),
	n(X,P3,N),
	rc(X,P3,P2,RC).
det(X,P1,P2,all(X,(P2->P1)),determiner(all)).
det(X,P1,P2,exist(X,(P2&P1)),determiner(a)).
det(X,P1,P2,exist(X,(P2&P1)),determiner(an)).
det(X,P1,P2,exist(X,(P2&P1)),determiner(the)).
det(X,P1,P2,exist(X,(P2&P1)),determiner(some)).
n(X,Y,noun(N)):-
	Y=..[N,X].
rc(X,P1,(P1&P2),rel_clause(relative(_),VP)):-
	vp(X,P2,VP).
rc(X,P1,(P1&P2),rel_clause(relative(_),NP,verb(V))):-
	P3=..[V,Y,X],
	np(Y,P3,P2,NP).
	
vp(X,P,verb_phrase(verb(V),NP)):-
	trans(V),
	P1=..[V,X,Y],
	np(Y,P1,P,NP).
vp(X,P,verb_phrase(verb(V))):-
	intrans(V),
	P=..[V,X].
vp(X,P,verb_phrase(verb(_),adjective(ADV))):-
	P=..[ADV,X].
translate(X):-
	parse(Y),
	ss(X,Y).
	
	
	

	
/*ss(sentence(NP,VP),F):-
	np(X,NP,VP,F).
np(X,noun_phrase(DET,N),VP,S):-
	det(X,DET,S1,S2,S),
	n(X,N,S1).
det(Var,determiner(all),S1,S2,all(Var,(S1->S2))).*/
