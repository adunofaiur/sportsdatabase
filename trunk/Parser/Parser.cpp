//Parser.cpp
//Contains recursive descent parser


//Do stuff here

#ifndef PASRER_CPP
#define PARSER_CPP

#include "Table.cpp"
#include "Token.h"
#include <iostream>
#include <vector>
#include "Lexer.cpp"

using namespace std;

int iter=0;

Token NoToken = *(new Token());


vector<string> NAMES;
vector<Table*> TABLES;

void UnEat(int Eaten = 1){//"UnEats" a token.
    iter-=Eaten;
}

Token Eat(vector<Token>& input){//"Eats" a token from the input and returns it.
    iter++;
    if(input.size()>iter-1){
        Token temp = input[iter-1];
        return temp;
    }
	return NoToken;
}

bool Match(vector<Token>& input, Token t){
    Token tok = Eat(input);
    if((tok.get_id()==t.get_id() && tok.get_val()==t.get_val()) || (tok.get_id()==t.get_id() && t.get_val()=="")) return true;
    return false;
}

Table* Identifier(vector<Token>& input);
Table* Expression(vector<Token>& input);
Table* Atomic(vector<Token>& input);
Table* Selection(vector<Token>& input);
Table* Projection(vector<Token>& input);
Table* Renaming(vector<Token>& input);
Table* Union(vector<Token>& input);
Table* Difference(vector<Token>& input);
Table* Product(vector<Token>& input);
vector<string> Condition(vector<Token>& input);
vector<string> Comparison(vector<Token>& input);
vector<string> Conjunction(vector<Token>& input);



bool Open(vector<Token>& input){//Opens a Table
    Token op('c', "OPEN");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
	if(input[iter].get_id()!='i') return false; //Bad format
	string toOpen = input[iter].get_val();
	for(int i=0; i<NAMES.size(); i++){
		if(toOpen.compare(NAMES[i])==0) return false;//Table already opened.
	}
	Table* opened = new Table();
	opened->setName(toOpen);
	bool ret = opened->read();
	if(!ret) return false;
	NAMES.push_back(toOpen);
	TABLES.push_back(opened);
	return true;
}

bool Close(vector<Token>& input){//Close a currently open table
    Token op('c', "CLOSE");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
	Table* close = Identifier(input);
	if(close ==NULL) return false;

	else{
		string Key = close->getName();
		for(int i=0; i<NAMES.size(); i++){
			if(Key.compare(NAMES[i])==0){
				TABLES.erase(TABLES.begin() + i);
				NAMES.erase(NAMES.begin() + i);
				return true;
			}
		}
	}
    return false;
}

bool Write(vector<Token>& input){//Write a currently open table
    Token op('c', "WRITE");
    if(!Match(input,op)){
        UnEat();
        return false;
    }
	if(input[iter].get_id()!='i') return false; //Bad format
	string toWrite = input[iter].get_val();
	for(int i=0; i<TABLES.size(); i++){
		if(toWrite.compare(NAMES[i])==0){
			TABLES[i]->save();
			return true;
		}
	}
	return false;
}
bool Show(vector<Token>& input){//Display a table
    Token op('c', "SHOW");
    if(!Match(input, op)){
        UnEat();
        return false;
    }
    Table* tbl = Atomic(input);
	if(tbl==NULL) return false;
	tbl->show();
	return true;
}
bool Exit(vector<Token>& input){//Do nothing
    Token op('c', "EXIT");
    bool ret =  Match(input, op);
    if(!ret) return false;
    return true;
}
bool Insert(vector<Token>& input){//Done
    Token insert('c', "INSERT");
    Token into('c', "INTO");
    if((!(Match(input, insert) && Match(input, into)))) return false;
	
	Table* insertee = Identifier(input);
	if(insertee==NULL) return false;
	
    Token values('c', "VALUES");
    Token from('c', "FROM");
    if((!(Match(input, values) && Match(input, from)))) return false;    
	Token relation('c', "RELATION");
	if(Match(input, relation)){
		Table* inserting = Expression(input);
		return insertee->setUnion(*inserting);
	}else{
		UnEat();
		vector<string> fields;
		Token delimit('h', "");
		Token comma('h', ",");
		if(!Match(input, delimit)) return false;
		Token t = Eat(input);
		if(t.get_id()!='i') return false;
		fields.push_back(t.get_val());
		while(Match(input, comma)){
			t = Eat(input);
			if(t.get_id()!='i') return false;
			fields.push_back(t.get_val());
		}UnEat();
		if(!Match(input, delimit)) return false;
		return insertee->addEntry(fields);
	}
}
/*Table* Update(vector<Token>& input){
    Token update('c', "UPDATE");
    if((!Match(input, update))){
        UnEat();
        return NULL;
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
}*/
bool Create(vector<Token>& input){//Finished function
    Token create('c', "CREATE");
    if(!Match(input, create)){
        UnEat();
        return false;
    }
    Token table('i', "TABLE");
	Token delimit('h', "");
    Token comma('h',",");	
	if(!Match(input, table)) return false;	
	Token nTok = Eat(input);
	if(nTok.get_id()!='i') return false;	
	string name = nTok.get_val();
	for(int i=0; i< TABLES.size(); i++){
		if(name.compare(NAMES[i])==0){//Table already exists
			return false;
		}
	}

	if(!Match(input, delimit)) return false;
	Token field = Eat(input);
	Token type = Eat(input);
	if(field.get_id()!='i' || type.get_id()!='i') return false;
	vector<string> fields;
	fields.push_back(name);
	fields.push_back(field.get_val());

    while(Match(input, comma)){
        field = Eat(input);
		type = Eat(input);
		if(field.get_id()!='i' || type.get_id()!='i') return false;
		fields.push_back(field.get_val());
    }
	UnEat();
	if(!Match(input, delimit)) return false;
	Table* temp = new Table(fields);
	NAMES.push_back(name);
	TABLES.push_back(temp);
	temp->show();
	return true;
}
/*bool Delete(vector<Token>& input){
    Token delet('c', "DELETE");
    Token from('c', "FROM");
    if(!(Match(input, delet) && Match(input, from))) return false;
	Table* tbl = Identifier(input);
	if(tbl==NULL) return false;
    Token where('c', "WHERE");
	if(!Match(input, where)) return false;
	
    bool ret = Match(input, from) && Identifier(input) && Match(input, where) && Condition(input);
    return ret;
}*/


Table* Atomic(vector<Token>& input){//Alias for expression
   return Expression(input);
}
vector<string> Comparison(vector<Token>& input){//Finished function
	vector<string> ret;
	ret.push_back("");
	ret.push_back("");
	ret.push_back("");
	Token t1 = Eat(input);
	if(t1.get_id()=='h'){
		if(t1.get_val()=="("){
			UnEat();
			return Condition(input);
		}
		return ret;
	}
	Token t2 = Eat(input);
	Token t3 = Eat(input);
	if(t1.get_id()!='i' || t2.get_id()!='o' || t3.get_id()!='i') return ret;
	if(t2.get_val()!="==" && t2.get_val()!=">" && t2.get_val()!="<" && t2.get_val()!=">=" && t2.get_val()!="<=" && t2.get_val()!="!=") return ret; 
	ret[0] = t1.get_val();
	ret[1] = t2.get_val();
	ret[2] = t3.get_val();
	return ret;
}

vector<string> Conjunction(vector<Token>& input){//Finished function
    vector<string> first = Comparison(input);
	if(first[0]=="") return first;
    Token op('o', "&&");
    while(Match(input, op)){
        vector<string> next = Comparison(input);
		if(next[0]=="") return next;
		first.push_back("(");
		first.push_back("&&");
		first.insert(first.end(), next.begin(), next.end());
		first.push_back( ")");
    }UnEat();    
    return first;
}

vector<string> Condition(vector<Token>& input){//Finished function
	vector<string> err;
	err.push_back("");
	err.push_back("");
	err.push_back("");

    Token delimit('h', "");
    if(!Match(input, delimit)) return err;
	vector<string> ret = Conjunction(input);
	if(ret[0]=="") return err;
	ret.insert(ret.begin(), "(");
    Token op('o', "||");
    while(Match(input, op)){
        vector<string> next = Conjunction(input);
		if(next[0]=="") return err;
		ret.push_back("||");
		ret.push_back("(");
		ret.insert(ret.end(), next.begin(), next.end());
		ret.push_back( ")");
    }UnEat();    
	ret.push_back( ")");
    if(!Match(input, delimit)) return err;
    return ret;
}
Table* SelectParse(vector<string>& logic, Table& begin, Table& curr, int& it){//Probably finished
	Table* temp = new Table(begin);
	string name1 = "";
	for(int i=it; i< logic.size(); i++){
		if(logic[i]=="(") ++it;
		else if(logic[i]==")"){ ++it; return temp;}
		else if(logic[i]=="!="){*temp = *(begin.rowQuerry(logic[i+1], name1, '=')); it+=3; i++;}
		else if(logic[i]==">"){*temp = *(begin.rowQuerry(logic[i+1], name1, '>')); it+=3; i++;}
		else if(logic[i]=="=="){*temp = *(begin.rowQuerry(logic[i+1], name1, '=')); it+=3; i++; temp -> show();}
		else if(logic[i]==">"){*temp = *(begin.rowQuerry(logic[i+1], name1, '<')); it+=3; i++;}
		else if(logic[i]==">="){*temp = *(begin.rowQuerry(logic[i+1], name1, '>')); temp->setUnion(*(begin.rowQuerry(logic[i+1], name1, '='))); it+=3; i++;}
		else if(logic[i]=="<="){*temp = *(begin.rowQuerry(logic[i+1], name1, '<')); temp->setUnion(*(begin.rowQuerry(logic[i+1], name1, '='))); it+=3; i++;}
		else if(logic[i]=="&&"){*temp = *(SelectParse( logic, begin, curr, ++it)); curr.setIntersection(*temp); *temp = curr;}
		else if(logic[i]=="||"){*temp = *(SelectParse( logic, begin, curr, ++it)); curr.setUnion(*temp); *temp = curr;}
		else if(i==it) name1 = logic[i];
		else return NULL;
	}
	return NULL;
}

Table* Selection(vector<Token>& input ){//Finished function
    Token op('e', "select");
     if(!Match(input, op)){
        UnEat();
        return NULL;
    }
	vector<string> logic = Condition(input);
	if(logic[0]=="") return NULL;
	Table* tbl = Atomic(input);
	Table current = *tbl;
	int it = 0;
	Table* result = SelectParse(logic, *tbl, current, it);
	return result;
}

Table* Projection(vector<Token>& input ){//Finished function
    Token op('e', "project");
    if(!Match(input, op)){
        UnEat();
        return NULL;
    }
    Token delimit('h', "");
    Token comma('h', ",");
    bool ret = Match(input, delimit);
	vector<string> fields;
	if(input[iter].get_id()!='i') return NULL;
	fields.push_back(input[iter].get_val());
    while(ret && Match(input, comma)){
        ret = ret && (input[iter].get_id()=='i');
		Token tok = Eat(input);
		fields.push_back(tok.get_val());
    }
	Table* tbl = Identifier(input);
	if(tbl==NULL) return NULL;
	return tbl->projection(fields);
}

Table* Renaming(vector<Token>& input ){//Finished function
    Token op('e', "rename");
     if(!Match(input, op)){
        UnEat();
		return NULL;
    }
	vector<string> fields;
	Token delimit('h', "");
	if(!Match(input, delimit)) return NULL;
	Token attr1 = Eat(input);
	if(attr1.get_id() != 'i') return NULL;
	fields.push_back(attr1.get_val());
    Token comma('h', ",");
    while(Match(input, comma)){
		attr1 = Eat(input);
		if(attr1.get_id() != 'i') return NULL;
		fields.push_back(attr1.get_val());
    }UnEat();
	if(!Match(input, delimit)) return NULL;
	Table* renamee = Atomic(input);
	if(renamee==NULL) return NULL;
	Table* ret = new Table(*renamee);
	if(ret->rename(fields)) return ret;
	return NULL;
}
Table* Union(vector<Token>& input ){//Finished function
    Token op('e', "+");
    Table* t1 =  Atomic(input);
	bool b = Match(input, op);
	Table* t2 = Atomic(input);
	if(!b) throw 1;
	if(t1 ==NULL || t2==NULL) return NULL;
	t1->setUnion(*(t2));
	return t1;
}

Table* Difference(vector<Token>& input ){//Finished function
    Token op('e', "-");
    Table* t1 =  Atomic(input);
	bool b = Match(input, op);
	Table* t2 = Atomic(input);
	if(!b) return NULL;
	if(t1 ==NULL || t2==NULL) return NULL;
    t1->setDifference(*(t2));
	return t1;
}

Table* Product(vector<Token>& input ){//Finished function
    Token op('e', "*");
    Table* t1 =  Atomic(input);
	bool b = Match(input, op);
	Table* t2 = Atomic(input);
	if(!b) return NULL;
	if(t1 ==NULL || t2==NULL) return NULL;
    t1->crossProduct(*(t2));
	return t1;
}

Table* Expression(vector<Token>& input){//Finished function
 	if(iter >= input.size()) return NULL;
	Token tok1 = input[iter];
	if(tok1.get_id() == 'e'){
		if(tok1.get_val()== "select"){
			return Selection(input);
		}else if(tok1.get_val()== "project"){
			return Projection(input);
		}else if(tok1.get_val()== "Rename"){
			return Renaming(input);
		}
	}
	if(iter<= input.size()-2){
		Token tok2 = input[iter+1];
		if(tok1.get_id() == 'e'){
			if(tok1.get_val()== "+"){
				return Union(input);
			}else if(tok1.get_val()== "-"){
				return Difference(input);
			}else if(tok1.get_val()== "*"){
				return Product(input);
			}
		}	
	}
	
	return Identifier(input);
}

Table* Identifier(vector<Token>& input){//Finished function
 	if(iter >= input.size()) return NULL;
	Token tok = Eat(input);
    if(tok.get_id()=='i'){
		string name = tok.get_val();
		for(int i=0; i< TABLES.size(); i++){
			if(name.compare(NAMES[i])==0){
				return TABLES[i];
			}
		}
		//Not an identifier.  Try literals.
		UnEat();
		return NULL;
	}
	UnEat();
	return NULL;
}

bool Querry(vector<Token>& input){//Finished function
	Token op('h', "<-");
	Token operand  = Eat(input);
	if(operand.get_id() != 'i') return false;
	string name = operand.get_val();
	if(!Match(input,op)) return false;
	Table* tbl = Expression(input);
	if(tbl==NULL) return false;
	for(int i=0; i<TABLES.size(); i++){
		if(name.compare(NAMES[i])==0){
			TABLES[i]= tbl;
			return true;
		}
	}
	tbl -> recalc();
	NAMES.push_back(name);
	TABLES.push_back(tbl);
	return true;
}

bool Command(vector<Token>& input){//Finished function
	Token op = input[0];
	if(op.get_id()!='c') return false;
	if(op.get_val()=="OPEN") return Open(input);
	if(op.get_val()=="CLOSE") return Close(input);
	if(op.get_val()=="EXIT") return Exit(input);
	if(op.get_val()=="WRITE") return Write(input);
	if(op.get_val().compare("SHOW")==0) return Show(input);
	if(op.get_val().compare("CREATE")==0) return Create(input);
	if(op.get_val()=="INSERT") return Insert(input);
//	if(op.get_val()=="UPDATE") return Update(input);
//	if(op.get_val()=="DELETE") return Delete(input);
	return false;
}
bool Parse(vector<Token>& input){//Finished function
//		for(int i=0; i<input.size(); i++){
//			cout<<"TOKEN"<<i<<" :  {"<<input[i].get_id()<<", "<<input[i].get_val()<<"}\n";
//		}
	if(input[0].get_id() == 'c') return Command(input);
	else return Querry(input);
}


#endif

int main(){
	while(true){
		iter = 0;
		cout<<"Please give your input: \n";
		string input;
		getline(cin, input);
		vector<Token> toks = tokenize(input);
		bool result = Parse(toks);
		if(!result) cout<<"Fail.\n";
		else cout<<"Success!\n";
		if(toks[0].get_val()=="EXIT") break;
	}
}
