//
//  Token.cpp
//  delimeter_inserter
//
//  Created by Matthew Carrasco on 9/14/13.
//  Copyright (c) 2013 Matthew Carrasco. All rights reserved.
//

#include "Token.h"

Token::Token(){//Default Token
	type = '#';
    val = "";
}
Token::Token(string new_val){
    val = new_val;
}
Token::Token(char new_type, string new_val){
    type = new_type;
    val = new_val;
}