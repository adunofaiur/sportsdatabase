//Lexer.cpp
//Defines Lexer.  Take Plain-text input, convert to token stream

//This function adds spaces to delineate future tokens

#include <string>
string add_spaces(string s){
    for (auto i = s.begin(); i != s.end()-1; i++){
   
        if (*i == '"' || *i == '(' || *i == ')' || *i == ';' || ','){
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