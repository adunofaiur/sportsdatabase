//Parser.cpp
//Contains recursive descent parser


//Do stuff here

#ifndef PASRER_CPP
#define PARSER_CPP

#include "Token.h"

#include <vector>

extern int iter=0;

void UnEat(int Eaten = 1){//"UnEats" a token.
    iter-=Eaten;
}

Token Eat(vector<Token>& input){//"Eats" a token from the input and returns it.
    iter++;
    if(input.size()>iter-1){
        Token temp = input[iter];
        return temp;
    }
}

bool Match(vector<Token>& input, Token t){
    Token tok = Eat(input);
    if((tok.get_id()==t.get_id() && tok.get_val()==t.get_val()) || (tok.get_id()==t.get_id() && t.get_val()=="")) return true;
    return false;
}

bool Identifier(vector<Token>& input);
bool Expression(vector<Token>& input);
bool Atomic(vector<Token>& input);
bool Selection(vector<Token>& input);
bool Projection(vector<Token>& input);
bool Renaming(vector<Token>& input);
bool Union(vector<Token>& input);
bool Difference(vector<Token>& input);
bool Product(vector<Token>& input);
bool Condition(vector<Token>& input);
bool Comparison(vector<Token>& input);
bool Conjunction(vector<Token>& input);
bool Comparison(vector<Token>& input);

bool Open(vector<Token>& input){
    Token op('c', "OPEN");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
    bool ret = Identifier(input);
    return ret;
}

bool Close(vector<Token>& input){
    Token op('c', "CLOSE");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
    bool ret = Identifier(input);
    return ret;
}

bool Write(vector<Token>& input){
    Token op('c', "WRITE");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
    bool ret = Identifier(input);
    return ret;
}
bool Show(vector<Token>& input){
    Token op('c', "SHOW");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
    bool ret = Atomic(input);
    if(!ret) UnEat();
    return ret;
}
bool Exit(vector<Token>& input){
    Token op('c', "EXIT");
    bool ret =  Match(input, op);
    if(!ret) UnEat();
    return ret;
}
bool Insert(vector<Token>& input){
    Token insert('c', "INSERT");
    if((!Match(input, insert))){
        UnEat();
        return false;
    }
    Token into('c', "INTO");
    Token values('c', "VALUES");
    Token from('c', "FROM");
    Token relation('c', "RELATION");
    Token comma('h', ",");
    bool base= Match(input, into) && Identifier(input)&&Match(input, values) && Match(input, from);
    bool ret = base && Identifier(input);
    int eaten = 0;
    while(ret && Match(input, comma)){
        ++eaten;
        ret = ret && Identifier(input);
    }
    if(!ret){
        UnEat(eaten);
        ret = base && Match(input, relation) && Expression(input);
        if(!ret) UnEat(4);
    }
    return ret;
}
bool Update(vector<Token>& input){
    Token update('c', "UPDATE");
    if((!Match(input, update))){
        UnEat();
        return false;
    }
    Token set('c', "SET");
    Token where('c', "WHERE");
    Token comma('h', ",");
    Token eq('o', "=");
    bool ret = Identifier(input) && Match(input, set)&& Identifier(input) && Match(input, eq) && Identifier(input);
    while(ret&&Match(input, comma)){
        ret = ret&& Identifier(input) && Match(input, eq) && Identifier(input);
    }
    ret = ret && Match(input, where) && Condition(input);
    return ret;
}
bool Create(vector<Token>& input){
    Token create('c', "CREATE");
    if(!Match(input, create)){
        UnEat();
        return false;
    }
    Token table('c', "TABLE");
    Token primary('c', "PRIMARY");
    Token key('c', "KEY");
    Token delimit('h', "");
    Token comma('h',",");
    bool ret = Match(input, table);
    ret = ret && Match(input, primary) && Match(input, key) && Identifier(input) && Identifier(input);
    while(ret && Match(input, comma)){
        ret = ret && Identifier(input) && Identifier(input);
    }
    ret = ret && Match(input, delimit)&&Identifier(input);
    while(ret && Match(input, comma)){
        ret = ret && Identifier(input);
    }
    return ret;
}
bool Delete(vector<Token>& input){
    Token delet('c', "DELETE");
    if(!Match(input, delet)){
        UnEat();
        return false;
    }
    Token from('c', "FROM");
    Token where('c', "WHERE");
    bool ret = Match(input, from) && Identifier(input) && Match(input, where) && Condition(input);
    return ret;
}


bool Atomic(vector<Token>& input){
    return Identifier(input) || Expression(input);
}
bool Comparison(vector<Token>& input){
    Token op('o', "");
    return Identifier(input) && Match(input, op) && Identifier(input);
}

bool Conjunction(vector<Token>& input){
    bool ret = Comparison(input);
    Token op('o', "&&");
    int eaten = 1;
    while(ret && Match(input, op)){
        ++eaten;
        ret = ret && Comparison(input);
    }
    if(!ret) UnEat(eaten);
    
    return ret;
}

bool Condition(vector<Token>& input){
    Token delimit('h', "");
    bool ret = (Match(input, delimit) && Conjunction(input) );
    Token op('o', "||");
    int eaten = 1;
    while(ret && Match(input, op)){
        ++eaten;
        ret = ret || Conjunction(input);
    }
    if(!ret) UnEat(eaten);
    return ret;
}


bool Selection(vector<Token>& input ){
    Token op('e', "select");
    return Match(input, op) && Condition(input) && Atomic(input);
}

bool Projection(vector<Token>& input ){
    Token op('e', "project");
    Token delimit('h', "");
    Token comma('h', ",");
    bool ret =  Match(input, op) && Match(input, delimit)&&Identifier(input);
    int eaten = 1;
    while(ret && Match(input, comma)){
        ++eaten;
        ret = ret && Identifier(input);
    }
    if(!ret) UnEat(eaten);
    return ret;
}

bool Renaming(vector<Token>& input ){
    Token op('e', "rename");
    Token delimit('h', "");
    Token comma('h', ",");
    bool ret =  Match(input, op) && Match(input, delimit)&&Identifier(input);
    int eaten = 1;
    while(ret && Match(input, comma)){
        ++eaten;
        ret = ret && Identifier(input);
    }
    if(!ret) UnEat(eaten);
    return ret;
}
bool Union(vector<Token>& input ){
    Token op('e', "+");
    bool ret = Atomic(input) && Match(input, op) && Atomic(input);
    if(!ret) UnEat();
    return ret;
}

bool Difference(vector<Token>& input ){
    Token op('e', "-");
    bool ret = Atomic(input) && Match(input, op) && Atomic(input);
    if(!ret) UnEat();
    return ret;
}

bool Product(vector<Token>& input ){
    Token op('e', "*");
    bool ret = Atomic(input) && Match(input, op) && Atomic(input);
    if(!ret) UnEat();
    return ret;
}

bool Expression(vector<Token>& input){
    return Atomic(input) || Selection(input )|| Projection(input) || Renaming(input)||Union(input)||Difference(input)||Product(input);
}

bool Identifier(vector<Token>& input){
    if(Eat(input).get_id()=='i') return true;
    UnEat();
    return false;
}

bool Querry(vector<Token>& input){
    Token op('h', "<-");
    bool ret =  Identifier(input) && Match(input, op) && Expression(input);
    if(!ret) UnEat();
    return ret;
}

bool Command(vector<Token>& input){
    return Open(input) || Close(input) || Write(input) || Exit(input) || Show(input) || Create(input) || Insert(input) || Update(input) || Delete(input);
}
bool Parse(vector<Token>& input){
    return Querry(input) || Command(input);
}


#endif
