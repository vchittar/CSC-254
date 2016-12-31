#include <iostream>
//#include <cstdlib>
//#include <cstring>
//#include <cctype>
//#include <stdio.h>
#include "scan.hpp"

std::string token_image;

using namespace std;

 token scan() {
    static char c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
	token_image = "";

    /* skip white space */
    while (isspace(c)) {
        c = std::cin.get();
    }
    if (cin.fail())
        return t_eof;

    if (isalpha(c)) {
        do {
            token_image += c;
            c = std::cin.get();
			//i++;
        } 
        while (isalpha(c) || isdigit(c) || c == '_');
        //token_image[i] = '\0';
		
		//cout << "Token image is"<<token_image;

        if (token_image == "read") 
        	return t_read;

        else if (token_image == "write") 
        	return t_write;
		
		else if (token_image == "if"){
			//cout << "if token read";
			return t_if;
		}
		else if (token_image == "fi")
			return t_fi;
		else if (token_image == "do")
			return t_do;
		else if (token_image == "od")
			return t_od;
		else if (token_image == "check")
			return t_check;

        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image += c;
            c = std::cin.get();
			//i++;
        } 
        while (isdigit(c));
        //token_image[i] = '\0';
		//cout << "Token image is currently "<<token_image;
        return t_literal;
    } 
    else switch (c) {
        case ':':
            if (c = std::cin.get() != '=') {
                cout << (cerr, "error\n"); //changed from stderr to cerr 
                throw "threw";

            } else {
                c = std::cin.get();
                return t_gets;
            }	
            	break;
		case '=':
			if(c = std::cin.get() != '='){
				cout << (cerr, "error\n"); //changed from stderr to cerr 
                throw "threw";
			}else{
				c = std::cin.get();
				return t_eqv;
			}
			break;
		
		case '<':
			if(c = std::cin.get() == '='){
				c = std::cin.get();
				return t_es;
			}else if(c == '>'){
				c = std::cin.get();
				return t_ne;
			}else{
				c = std::cin.get();
				return t_smaller;
			}
			break;
		case '>':
			if(c = std::cin.get() == '='){
				c = std::cin.get();
				return t_el;
			}else{
				c = std::cin.get();
				return t_larger;
			}
			break;
		case '$':
			if(c = std::cin.get() == '$'){
				c = std::cin.get();
				return t_eof;
			}else{
				cout << (cerr, "error\n"); //changed from stderr to cerr 
                throw "threw";
			}
			break;
        case '+': c = std::cin.get(); return t_add;
        case '-': c = std::cin.get(); return t_sub;
        case '*': c = std::cin.get(); return t_mul;
        case '/': c = std::cin.get(); return t_div;
        case '(': c = std::cin.get(); return t_lparen;
        case ')': c = std::cin.get(); return t_rparen;
        default:
            	cout << "error\n";
            	throw "404";
    }
}
