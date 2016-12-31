/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
*/

#include <iostream>
//#include "stdlib.h"
#include <iterator>
#include <vector>
#include <algorithm>
#include "scan.hpp"
using namespace std;

const std::string names[] = {"read", "write", "if", "fi", "do", "od", "check", "id", "literal", "gets", "ro",
                       "eqv", "Not_Eqv", "smaller", "larger", "<=", ">=", "add", "sub", "mul", "div", "lparen", "rparen", "eof"};

static token input_token;
std::string output = "";
std::string idF = "", idFO = "";

void error () {
    cout<< ("syntax error\n");
    throw "You're screwed";
}

void match (token expected) {
    if (input_token == expected) {
        //cout << "matched " << names[input_token];
        //if (input_token == t_id || input_token == t_literal)
        //    output +=  token_image;
        //cout<< ("\n");
        input_token = scan ();
    }else if(input_token == t_eof){
		cout << "ERROR - token not matched, and end of file is reached\n";
		return;
	}
    else{
		cout << "ERROR - token not matched. \n";
		input_token = scan();
		match(expected);
	}
}

void program ();
void stmt_list ();
void stmt ();
void relation ();
void expr ();
void expr_tail ();
void term ();
void term_tail ();
void factor_tail ();
void factor ();
void add_op ();
void mul_op ();
void rel_op ();

token firstP[] = {t_id, t_read, t_write, t_if, t_do, t_check};
token firstSL[] = {t_eof, t_fi, t_od, t_id, t_read, t_write, t_if, t_do, t_check};
token firstS[] = {t_id, t_read, t_write, t_if, t_do, t_check};
token firstR[] = {t_lparen, t_id, t_literal};
token firstE[] = {t_lparen, t_id, t_literal};
token firstT[] = {t_lparen, t_id, t_literal};
token firstF[] = {t_lparen, t_id, t_literal};
token firstET[] = {t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_rparen, t_id, t_read, t_write, t_if, t_do, t_check, t_eof};
token firstTT[] = {t_add, t_sub, t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_rparen, t_id, t_read, t_write, t_if, t_do, t_check, t_eof};
token firstFT[] = {t_mul, t_div, t_add, t_sub, t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_rparen, t_id, t_read, t_write, t_if, t_do, t_check, t_eof};
token firstAo[] = {t_add, t_sub};
token firstMo[] = {t_mul, t_div};
token firstRo[] = {t_eqv, t_ne, t_smaller, t_larger, t_es, t_el};

token followP[] = {};
token followSL[] = {t_eof, t_fi, t_od};
token followS[] = {t_eof, t_fi, t_od};
token followR[] = {t_eof, t_fi, t_od, t_rparen};
token followE[] = {t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_eof, t_fi, t_od, t_rparen};
token followT[] = {t_add, t_sub, t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_eof, t_fi, t_od, t_rparen};
token followF[] = {t_mul, t_div, t_add, t_sub, t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_eof, t_fi, t_od, t_rparen};
token followET[] = {t_eof, t_fi, t_od, t_rparen};
token followTT[] = {t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_eof, t_fi, t_od, t_rparen};
token followFT[] = {t_add, t_sub, t_eqv, t_ne, t_smaller, t_larger, t_es, t_el, t_eof, t_fi, t_od, t_rparen};
token followOp[] = {t_lparen, t_id, t_literal};



bool contains(token array[], token input, int size){
	int i = 0;
	while(i < size){
		if(array[i] == input)
			return true;
		i++;
	}
	return false;
}

//P
void program () {
	/*if (!(std::find(std::begin(firstP), std::end(firstP), input_token) != std::end(firstP) || std::find(std::begin(followP), std::end(followP), input_token) != std::end(followP))){
		cout << "Screw you! You're stupid enough to not be able to write a working P!"
		while (!(std::find(std::begin(firstP), std::end(firstP), input_token) != std::end(firstP) || std::find(std::begin(followP), std::end(followP), input_token) != std::end(followP) || input_token == t_eof)){
			input_token = scan();
		}
	}*/
	if(! (contains(firstP,input_token,sizeof(firstP)/4) || contains(followP, input_token,sizeof(followP)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstP,input_token,sizeof(firstP)/4) || contains(followP, input_token,sizeof(followP)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
		case t_if:
		case t_do:
		case t_check:
        case t_eof:
            cout<< ("predict program --> stmt_list eof\n");
            stmt_list ();
            match (t_eof);
            break;
        default: 
			//error ();
			return;
    }
}

//SL
void stmt_list () {
	if(! (contains(firstSL,input_token,sizeof(firstSL)/4) || contains(followSL, input_token,sizeof(followSL)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstSL,input_token,sizeof(firstSL)/4) || contains(followSL, input_token,sizeof(followSL)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
		case t_if:
		case t_do:
		case t_check:
            cout<< ("predict stmt_list --> stmt stmt_list\n");
            stmt ();
			//cout << "OUTPUT: "<<output;
            stmt_list ();
            break;
        //case t_eof:
		case t_fi:
		case t_od:
            cout << ("predict stmt_list --> epsilon\n");
            break;          /*  epsilon production */
		case t_eof:
			cout << "OUTPUT: "<<output;
			output = "";
			break;
        default: error ();
    }
}

//S
void stmt () {
	if(! (contains(firstS,input_token,sizeof(firstS)/4) || contains(followS, input_token,sizeof(followS)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstS,input_token,sizeof(firstS)/4) || contains(followS, input_token,sizeof(followS)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_id:{
            cout<< ("predict stmt --> id := R\n");
			//std::string id = token_image;
			output += ("(:= "+token_image);
            match (t_id);
			output += (" "+token_image);
			//cout << "id here is "<<id<<endl;
            match (t_gets);
			
            relation ();
			output += ")";
            break;
		}
        case t_read:
            cout<< ("predict stmt --> read id\n");
			output += ("(read ");
            match (t_read);
			output += (token_image+")");
            match (t_id);
            break;
        case t_write:
            cout<< ("predict stmt --> write R\n");
            match (t_write);
			output += "(write ";
            relation ();
			output += ")";
            break;
		case t_if:
			cout << "predict S -> if R SL fi\n";
			output+= "(if ";
			match(t_if);
			relation ();
			output+= " ";
			stmt_list ();
			output+= ")";
			match(t_fi);
			break;
		case t_do:
			cout << "predict S -> do SL od\n";
			output+= "(do ";
			match(t_do);
			stmt_list ();
			output+= ")";
			match(t_od);
			break;
		case t_check:
			cout << "predict S -> check R\n";
			output+= "(check ";
			match(t_check);
			relation ();
			output+= ")";
			break;
        default: error ();
    }
}

//R
void relation(){
	if(! (contains(firstR,input_token,sizeof(firstR)/4) || contains(followR, input_token,sizeof(followR)/4))){
		cout << "Wrong input detected. Trying to resume. Token is "<<input_token<<endl;
		while(! (contains(firstR,input_token,sizeof(firstR)/4) || contains(followR, input_token,sizeof(followR)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
	switch (input_token){
		case t_id:
        case t_literal:
        case t_lparen:
            cout<< ("predict R --> E ET\n");
            expr ();
            expr_tail ();
            break;
        default:
			cout << "wrong token! " << input_token;
			error ();
	}
}

//E
void expr () {
	if(! (contains(firstE,input_token,sizeof(firstE)/4) || contains(followE, input_token,sizeof(followE)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstE,input_token,sizeof(firstE)/4) || contains(followE, input_token,sizeof(followE)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout<< ("predict E --> term term_tail\n");
            term ();
            term_tail ();
            break;
        default: error ();
    }
}

//ET
void expr_tail () {
	if(! (contains(firstET,input_token,sizeof(firstET)/4) || contains(followET, input_token,sizeof(followET)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstET,input_token,sizeof(firstET)/4) || contains(followET, input_token,sizeof(followET)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
	switch (input_token){
		case t_eqv:
		case t_ne:
		case t_smaller:
		case t_larger:
		case t_es:
		case t_el:
			cout<< ("predict ET --> ro E\n");
			rel_op();
			expr();
		case t_id:
        case t_read:
        case t_write:
		case t_if:
		case t_fi:
		case t_do:
		case t_od:
		case t_check:
		case t_eof:
		case t_rparen:
			cout << ("predict ET --> epsilon\n");
            break;
		default: 
			cout << "Token type is "<<input_token;
			error ();
	}
}

//T
void term () {
	if(! (contains(firstT,input_token,sizeof(firstT)/4) || contains(followT, input_token,sizeof(followT)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstT,input_token,sizeof(firstT)/4) || contains(followT, input_token,sizeof(followT)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout<< ("predict T --> factor factor_tail\n");
            factor ();
            factor_tail ();
			output += idFO;
			idFO = "";
            break;
        default: error ();
    }
}

//TT
void term_tail () {
	if(! (contains(firstTT,input_token,sizeof(firstTT)/4) || contains(followTT, input_token,sizeof(followTT)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstTT,input_token,sizeof(firstTT)/4) || contains(followTT, input_token,sizeof(followTT)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_add:
        case t_sub:
            cout<< ("predict term_tail --> add_op term term_tail\n");
            add_op ();
            term ();
            term_tail ();
            break;
        case t_id:
        case t_read:
        case t_write:
		case t_if:
		case t_fi:
		case t_do:
		case t_od:
		case t_check:
		case t_eqv:
		case t_ne:
		case t_smaller:
		case t_larger:
		case t_es:
		case t_el:
		case t_eof:
		case t_rparen:
            cout<< ("predict term_tail --> epsilon\n");
            break;          /*  epsilon production */
        default: error ();
    }
}

//FT
void factor_tail () {
	if(! (contains(firstFT,input_token,sizeof(firstFT)/4) || contains(followFT, input_token,sizeof(followFT)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstFT,input_token,sizeof(firstFT)/4) || contains(followFT, input_token,sizeof(followFT)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_mul:
        case t_div:
            cout<< ("predict factor_tail --> mul_op factor factor_tail\n");
			//idF += "(";
            mul_op ();
            factor ();
			idF += ")";
			cout << "at FT, idF is "<<idF << endl;
            factor_tail ();
			//output += ")";
            break;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
		case t_if:
		case t_fi:
		case t_do:
		case t_od:
		case t_check:
		case t_eqv:
		case t_ne:
		case t_smaller:
		case t_larger:
		case t_es:
		case t_el:
        case t_eof:
			//idF += ")";
			
			idFO = (idFO + idF);
			cout << "at the end of FT, idFO is "<<idFO<<" and idF is "<<idF << endl;
			
			idF = "";
            cout<< "predict factor_tail --> epsilon, token = "<<input_token<<endl;
            break;          /*  epsilon production */
        default:
			cout << "Token here is "<<input_token;
			error ();
    }
}

//F
void factor () {
	if(! (contains(firstF,input_token,sizeof(firstF)/4) || contains(followF, input_token,sizeof(followF)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstF,input_token,sizeof(firstF)/4) || contains(followF, input_token,sizeof(followF)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_id :
            cout<< ("predict F --> id\n");
			idF += token_image;
            match (t_id);
            break;
        case t_literal:
            cout<< ("predict F --> literal\n");
			idF += token_image;
            match (t_literal);
            break;
        case t_lparen:
            cout<< ("predict F --> ( R )\n");
			//output += "(";
			idFO += idF;
			idF = "";
            match (t_lparen);
            relation ();
			//cout << "idFO is "<<idFO<<" and idF is "<<idF<<endl;
			//output += ")";
            match (t_rparen);
            break;
        default: error ();
    }
}


//ao
void add_op () {
	if(! (contains(firstAo,input_token,sizeof(firstAo)/4) || contains(followOp, input_token,sizeof(followOp)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstAo,input_token,sizeof(firstAo)/4) || contains(followOp, input_token,sizeof(followOp)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_add:
            cout<< ("predict add_op --> add\n");
			//output += ("(add "+idF);
            match (t_add);
            break;
        case t_sub:
            cout<< ("predict add_op --> sub\n");
            match (t_sub);
            break;
        default: error ();
    }
}

//mo
void mul_op () {
	if(! (contains(firstMo,input_token,sizeof(firstMo)/4) || contains(followOp, input_token,sizeof(followOp)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstMo,input_token,sizeof(firstMo)/4) || contains(followOp, input_token,sizeof(followOp)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
    switch (input_token) {
        case t_mul:
            cout<< ("predict mul_op --> mul\n");
			cout << "idF is currently "<<idF<<endl;
			idF = ("(* "+idF+" ");
            match (t_mul);
            break;
        case t_div:
            cout<< ("predict mul_op --> div\n");
            match (t_div);
            break;
        default: error ();
    }
}

//ro
void rel_op () {
	if(! (contains(firstRo,input_token,sizeof(firstRo)/4) || contains(followOp, input_token,sizeof(followOp)/4))){
		cout << "Wrong input detected. Trying to resume. \n";
		while(! (contains(firstRo,input_token,sizeof(firstRo)/4) || contains(followOp, input_token,sizeof(followOp)/4) || input_token == t_eof)){
			input_token = scan();
		}
	}
	switch(input_token){
		case t_eqv:
			cout << ("predict ro -> ==");
			match(t_eqv);
			break;
		case t_ne:
			cout << "predict ro -> <>";
			match(t_ne);
			break;
		case t_smaller:
			cout << "predict ro -> <";
			match(t_smaller);
			break;
		case t_larger:
			cout << "predict ro -> >";
			match(t_larger);
			break;
		case t_es:
			cout << "predict ro -> <=";
			match(t_es);
			break;
		case t_el:
			cout << "predict ro -> <=";
			match(t_el);
			break;
		default:
			error();
	}
}

main () {
    input_token = scan ();
    program ();
}
