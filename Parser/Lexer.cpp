//Lexer.cpp
//Defines Lexer.  Take Plain-text input, convert to token stream

//Call tokenizer on a string to get a vector of tokens.
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Token.h"
#include "Token.cpp"
#include "Parser.cpp"
using namespace std;








//Adds spaces before/after parenthesis, quotes, commas, and semicolons
string add_spaces(string s){
    for (string::iterator i = s.begin(); i != s.end()-1; i++){
        
        if (*i == '(' || *i == ')' || *i == ';' || *i == ','){
            if (*(i+1) != ' '){
                s.insert((i+1), ' ');
            }
            if (i != s.begin()){
                if (*(i-1) != ' '){
                    s.insert(i, ' ');
                }
            }
        }
        
        
    }
    return s;
}


Token make_token(string s){
    
    //Check to see if the string is a single special character
    //that is NOT an operand
    if (s == "(" || s == ")" || s == ";" || s == "," || "<-"){
        Token t = Token ('a', s);
        return Token('h', s);
    }
    //Checks to see if the string is an operator
    if (s == "&&" || s == "||" || s == "==" || s == "!=" || s== "<" || s== ">" || s== "<=" || s== ">=" || s=="="){
        return Token('o', s);
    }
    else if (s == "+" || s == "-" || s == "*" || s == "rename" ||
             s == "project" || s == "select"){
        return Token('e', s);
    }
    else if (s == "DELETE" || s == "VARCHAR" || s == "INTEGER" || s == "DELETE" || s== "INSERT" || s == "INTO" || s == "UPDATE" || s == "OPEN" || s == "CLOSE" || s == "WRITE" ||
             s == "EXIT" || s == "SHOW" || s == "CREATE" || s  == "VALUES" || s == "FROM" ||
             s == "RELATION" || s == "WHERE"){
        return Token('c', s);
    }
    else {
        string::iterator a = s.begin();
        string::iterator b = s.end();
        
        if (*a == '"' || *b == '"'){
            s.erase(a);
            s.erase(b);
        }
        
        return Token('i', s);
    }
    
}
vector<Token> tokenize(string s){
    
    
    s = add_spaces(s);
    
    vector<Token> t;
    vector<string> s2;
    //Splits the string into a vector of strings deliniteated by spaces
    istringstream iss(s);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(s2));
    /*
     for (auto i = 0; i < s2.size(); i++){
     cout << s2[i] << "\n";
     }*/
    for (int i = 0; i < s2.size(); i++){
        t.push_back(make_token(s2[i]));
    }
    return t;
}
int main() {
    string input;
	while(true){
		cout << "Enter strings to evaluate correctness: ";
		getline(cin, input);
        vector<Token> toks = tokenize(input);
		for(int i=0; i<toks.size(); i++){
			cout<<"TOKEN"<<i<<" :  {"<<toks[i].get_id()<<", "<<toks[i].get_val()<<"}\n";
		}
        bool correct = Parse(toks);
        cout << "Is correct?: " << correct << endl;
	}
}