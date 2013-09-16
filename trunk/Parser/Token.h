//
//  Token.h
//  delimeter_inserter
//
//  Created by Matthew Carrasco on 9/14/13.
//  Copyright (c) 2013 Matthew Carrasco. All rights reserved.
//

#ifndef __delimeter_inserter__Token__
#define __delimeter_inserter__Token__

#include <iostream>
#include <string>
using namespace std;

class Token{
private:
    char type;
    string val;
public:
	Token();
    Token(string new_val);
    Token(char new_type, string new_val);
    
    char get_id() { return type; }
    string get_val() {return val;}
    
};

#endif /* defined(__delimeter_inserter__Token__) */
