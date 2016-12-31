//CSC 254 Assignment 2
//Yufei (Edward) Du and Vishnu Chittari
//
//Modified from the C code provided on the assignment webpage
//See Readme for more information

#include <iostream>
#include <iterator>
#include <stack>
#include <algorithm>
#include "scan.hpp"
using namespace std;

//Corresponding names of tokens. It is from the original C code provided. I don't think our program use it anymore
const std::string names[] = {"read", "write", "if", "fi", "do", "od", "check", "id", "literal", "gets", "ro",
                       "eqv", "Not_Eqv", "smaller", "larger", "<=", ">=", "add", "sub", "mul", "div", "lparen", "rparen", "eof"};
					   
//Node: a binary tree data structure to store relations
struct Node {
	std::string value;
	Node* lc;
	Node* rc;
};

static token input_token;
std::string output = "";
//idF, idFO, idFL, and isright are deprecated variables used for storing relations. They are now replaced by the binary tree. We did not have time to take them out or comment them out, so they are left here.
std::string idF = "", idFO = "", idFL = "";
bool isright = false;
//The head of the binary tree
Node* treeHead = new Node;
//thestack is the stack storing the order of un-completed nodes/operators.
std::stack<Node*> thestack;

//When an un-recoverable error occurs, exit
void error () {
    cout<< ("syntax error\n");
    throw "";
}

//match function. Most of the code is from the original C code. Error recovery concepts are included.
void match (token expected) {
    if (input_token == expected) {
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
void printTree(Node* temp);

//FIRST and FOLLOW sets for all functions
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


//We could not find a built-in Arrays.contains() method like in Java, so we wrote one
//It's really hard to get the length of the array in this method, so it takes it in from the argument.
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
	//Error recovery mechanism as written in Textbook
	//All logical functions in this file have this mechanism.
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
            stmt_list ();
            match (t_eof);
            break;
        default: 
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
            stmt ();
            stmt_list ();
            break;
		case t_fi:
		case t_od:
            break;          /*  epsilon production */
		case t_eof:
			//print the output and then reset the output line
			cout << "OUTPUT: "<<output<<endl;
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
		//id := R
        case t_id:{
			output += ("(:= "+token_image);
            match (t_id);
			output += (" "+token_image);
            match (t_gets);
            relation ();
			//printTree to print the elements in the binary tree and reset the tree and stack
			printTree(treeHead);
			output += ")";
            break;
		}
		//read id
        case t_read:
			output += ("(read ");
            match (t_read);
			output += (token_image+")");
            match (t_id);
            break;
		//write R
        case t_write:
            match (t_write);
			output += "(write ";
            relation ();
			printTree(treeHead);
			output += ")";
            break;
		//if R SL fi
		case t_if:
			output+= "(if ";
			match(t_if);
			relation ();
			printTree(treeHead);
			output+= " ";
			stmt_list ();
			output+= ")";
			match(t_fi);
			break;
		//do SL od
		case t_do:
			output+= "(do ";
			match(t_do);
			stmt_list ();
			output+= ")";
			match(t_od);
			break;
		//check R
		case t_check:
			output+= "(check ";
			match(t_check);
			relation ();
			printTree(treeHead);
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
		//R->E ET
		case t_id:
        case t_literal:
        case t_lparen:
            expr ();
            expr_tail ();
            break;
        default:
			cout << "wrong token! ";
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
		//E->T TT
        case t_id:
        case t_literal:
        case t_lparen:
            //cout<< ("predict E --> term term_tail\n");
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
		//ET->ro E
		case t_eqv:
		case t_ne:
		case t_smaller:
		case t_larger:
		case t_es:
		case t_el:
			rel_op();
			expr();
		//Epsilon
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
            break;
		default: 
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
		//T->F FT
        case t_id:
        case t_literal:
        case t_lparen:
            factor ();
            factor_tail ();
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
		//TT -> ao T TT
        case t_add:
        case t_sub:
            add_op ();
            term ();
            term_tail ();
            break;
		//Epsilon
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
		//FT -> mo F FT
        case t_mul:
        case t_div:
            mul_op ();
            factor ();
            factor_tail ();
            break;
		//Epsilon
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
            break;          /*  epsilon production */
        default:
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
		//F -> id
        case t_id :
			//Peek the top of the stack. 
			//If that node doesn't have left child (which means that it's empty), let this id be its left child
			//Else let the id be its right child.
			if((thestack.top())->lc == NULL){
				Node* temp = new Node;
				temp -> value = token_image;
				temp->lc=NULL;
				temp->rc=NULL;
				(thestack.top())->lc = temp;
				
			}else{
				Node* temp = new Node;
				temp -> value = token_image;
				temp->lc=NULL;
				temp->rc=NULL;
				(thestack.top())->rc = temp;
				thestack.pop();
			}
            match (t_id);
            break;
		//F -> lilteral
        case t_literal:
			//Peek the top of the stack. 
			//If that node doesn't have left child (which means that it's empty), let this literal be its left child
			//Else let the literal be its right child.
			if((thestack.top())->lc == NULL){
				Node* temp = new Node;
				temp -> value = token_image;
				temp->lc=NULL;
				temp->rc=NULL;
				(thestack.top())->lc = temp;
			}else{
				Node* temp = new Node;
				temp -> value = token_image;
				temp->lc=NULL;
				temp->rc=NULL;
				(thestack.top())->rc = temp;
				thestack.pop();
			}
            match (t_literal);
            break;
		//F -> (R)
        case t_lparen:{
			//Peek the top of the stack.
			//If that node doesn't have left child, create an empty left child and push it to the stack
			//Else create an empty right child, pop the current node on the stack, and push this node (the right child of the original node)
			if((thestack.top())->lc == NULL){
				Node* temp = new Node;
				temp->lc = NULL;
				temp->rc = NULL;
				(thestack.top())->lc = temp;
				thestack.push((thestack.top())->lc);
			}else{
				Node* temp = new Node;
				(thestack.top())->rc = temp;
				temp->lc = NULL;
				temp->rc = NULL;
				thestack.pop();
				thestack.push(temp);
			}
            match (t_lparen);
            relation ();
            match (t_rparen);
            break;
		}
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
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "+";
            match (t_add);
            break;
        case t_sub:
			(thestack.top())->value = "-";
			//Assign the operator to the peek of the stack
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
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "*";
            match (t_mul);
            break;
        case t_div:
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "/";
			idF = ("(/ "+idF+" ");
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
			match(t_eqv);
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "==";
			break;
		case t_ne:
			match(t_ne);
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "<>";
			break;
		case t_smaller:
			match(t_smaller);
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "<";
			break;
		case t_larger:
			match(t_larger);
			//Assign the operator to the peek of the stack
			(thestack.top())->value = ">";
			break;
		case t_es:
			match(t_es);
			//Assign the operator to the peek of the stack
			(thestack.top())->value = "<=";
			break;
		case t_el:
			match(t_el);
			//Assign the operator to the peek of the stack
			(thestack.top())->value = ">=";
			break;
		default:
			error();
	}
}

//printTree recursively generate the output of the current relation and reset the tree and stack in the end
void printTree(Node* temp){
	if(temp==NULL){
		return;
	}
	output += ("(" + temp->value+" ");
	if(temp->lc != 0){
		printTree(temp->lc);
		printTree(temp->rc);
	}else{
		treeHead = new Node;
		treeHead->lc = NULL;
		treeHead->rc = NULL;
		thestack.push(treeHead);
	}
	output+=")";
}

//Initialize the stack by adding the head node to it.
main () {
	thestack.push(treeHead);
    input_token = scan ();
    program ();
}
