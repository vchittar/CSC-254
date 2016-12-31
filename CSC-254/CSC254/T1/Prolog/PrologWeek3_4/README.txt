Name: Yufei (Edward) Du
Assignment: Prolog Week 3-4
Date of submission: 11/6/2015

How to run the program:
read in input: read_in(X). Then use keyboard input.
read and parse: parse(X). Then use keyboard input.
read, parse, and translate: translate(X). Then use keyboard input.
just parse: sentence(X,input,[]).
just translate: ss(X,input).

There are 3 main parts in the program:
1. Input Reader (Used the code in Chapter 5 of the textbook)
2. Parser (Used the algorithm in the textbook and CB's tutorial page)
3. Translator (Used the algorithm in CB's tutorial page)

This program will read in a line of English sentence from keyboard input, parse it into a parse tree, and then translate the sentence into logical expression (if possible). The reader is done by the author of the textbook, so I didn't pay much attention. The parser parses the list of words into a parse tree. It is done by parsing each word according to English grammar with a list of known facts of nouns, determiners, verbs, adjectives, and punctuation marks. It is similar to a CFG. Each sentence should follow one of the following grammar: 
Sentence = Noun Phrase + Verb Phrase
Noun Phrase = Determiner + Noun
Noun Phrase = Noun
Noun Phrase = Determiner + Noun + Relative Clause
Verb Phrase = Verb
Verb Phrase = Verb + Noun Phrase
Verb Phrase = Beverb + Adjective
Verb Phrase = Beverb + Noun Phrase
Relative Clause = Relative word (that, who, etc.) + Verb Phrase
Relative Clause = Relative word (that, whom, etc.) + Noun Phrase + Verb

The translator then translate the parse tree to a logical expression. It should accept the following type of sentences:
Sentence = Noun Phrase + Verb Phrase
Noun Phrase = Determiner + Noun
Noun Phrase = Determiner + Noun + Relative Clause
Relative Clause = Relative word (that, who, etc.) + Verb Phrase
Relative Clause = Relative word (that, whom, etc.) + Noun Phrase + Verb
Verb Phrase = Verb
Verb Phrase = Verb + Noun Phrase
Verb Phrase = Beverb + Adjective


List of file: 
test.pl - Main program
README.txt - Read me
WRITEUP.docx - Write up

Extra credit: 
Not sure if this count, but my program can handle vowels.

Feedback:
It will be really really helpful if there is a list of grammars that the program should be able to handle. Right now I still have question about "Should my program also handle this type of grammar?". 