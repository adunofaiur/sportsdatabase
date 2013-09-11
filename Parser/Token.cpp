//Token.cpp
//Defines Token Class

struct Token{
	char kind;
	char value;

	Token() : kind(' '), value(' '){}  
	Token(char k) : kind(k), value(' '){}
	Token(char k, char v) : kind(k), value(v){}
};