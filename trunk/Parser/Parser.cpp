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
bool Update(vector<Token>& input){
    Token update('c', "UPDATE");
    if((!Match(input, update))){
        UnEat();
        return NULL;
    }
	Table* tbl = Identifier(input);
	if(tbl==NULL) return false;
    Token set('c', "SET");
	if(!Match(input, set)) return false;
	vector<string> old_vals;
	vector<string> new_vals;
    Token comma('h', ",");
    Token eq('o', "=");
	Token tok = Eat(input);
	if(tok.get_id()!='i') return false;
	old_vals.push_back(tok.get_val());
	if(!Match(input, eq)) return false;
	tok = Eat(input);
	if(tok.get_id()!='i') return false;
	new_vals.push_back(tok.get_val());
	while(Match(input, comma)){
		tok = Eat(input);
		if(tok.get_id()!='i') return false;
		old_vals.push_back(tok.get_val());
		if(!Match(input, eq)) return false;
		tok = Eat(input);
		if(tok.get_id()!='i') return false;		
		new_vals.push_back(tok.get_val());
	}
	
    Token where('c', "WHERE");
	if(!Match(input, where)) return false;
	vector<string> logic = Condition(input);
	Table current = *tbl;
	int it = 0;
	Table* to_update = SelectParse(logic, *tbl, current, it);
	if(to_update==NULL) return false;
	for(int i=0; i<to_update->size(); i++){
		tbl->update(to_update->getRow(i), old_vals, new_vals);
	}

}
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
bool Delete(vector<Token>& input){
    Token delet('c', "DELETE");
    Token from('c', "FROM");
    if(!(Match(input, delet) && Match(input, from))) return false;
	Table* tbl = Identifier(input);
	if(tbl==NULL) return false;
    Token where('c', "WHERE");
	if(!Match(input, where)) return false;
	vector<string> logic = Condition(input);
	Table current = *tbl;
	int it = 0;
	Table* to_delete = SelectParse(logic, *tbl, current, it);
	if(to_delete==NULL) return false;
	for(int i=0; i<to_delete->size(); i++){
		tbl->removeRow(to_delete->getRow(i));
	}
    return true;
}


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
	if(op.get_val()=="UPDATE") return Update(input);
	if(op.get_val()=="DELETE") return Delete(input);
	return false;
}
bool Parse(vector<Token>& input){//Finished function
//		for(int i=0; i<input.size(); i++){
//			cout<<"TOKEN"<<i<<" :  {"<<input[i].get_id()<<", "<<input[i].get_val()<<"}\n";
//		}
	iter =0;
	if(input[0].get_id() == 'c') return Command(input);
	else return Querry(input);
}


#endif

int main(){

		cout << "Welcome to the database app.\n";
		cout << "You may use either SQL query language or the following command menu.\n";
		cout << "Please don't use spaces in fields\n(you may only create a table with the query language)\n";
	
		//Load relevant tables
		string table_command;
		vector<Token> tokens;
		
		table_command = "OPEN players";
		tokens = tokenize(table_command);
		bool maybe = Parse(tokens);
		cout << maybe << "\n";
		table_command = "OPEN teams";
		tokens = tokenize(table_command);
		maybe = Parse(tokens);
		cout << maybe << "\n";
		
		table_command = "OPEN sponsors";
		tokens = tokenize(table_command);
		maybe = Parse(tokens);
		cout << maybe << "\n";
		bool go_on = true;
		while(go_on){
		
			iter = 0;
			cout<<"Give your menu option: \n";
			int input;
			cin >> input;
			//Switch between -
			/*
				Add player     1
				Add sponsor    2
				Add team       3
				remove pst      4, 5, 6  
				Update pst	   7, 8, 9
				List pst        10, 11, 12
				Search player (name, team) 13
				Search team (sponsor) 14
				Show  players by team - 15
				SQP - 16
				Exit - 17
				
			*/
			
			
			switch(input){
				case 1:{
				cout << "Enter: 'NAME NUMBER POSITION HEIGHT WEIGHT SPONSOR TEAM'\n";
				vector<string> values;
				string temp;
				for(int i=0; i<7; i++){
					cin	>> temp;
					values.push_back(temp);
				}
				table_command = "INSERT INTO players VALUES FROM ( " + values[0] + ", ";
				table_command += (values[1]) + ", " + (values[2]) + ", " + (values[3]) + (values[4]) + ", ";
				table_command += (values[5]) + ", " + (values[6]) + " )";
				tokens = tokenize(table_command);
				bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 2:{
				cout << "Enter: 'SPONSOR LOCATION PRODUCT'\n";
				vector<string> values;
				string temp;
				for(int i=0; i<3; i++){
					cin	>> temp;
					values.push_back(temp);
				}
				table_command = "INSERT INTO players VALUES FROM ( " + values[0] + ", ";
				table_command += (values[1]) + ", " + (values[2]) + ", " + (values[3]) + " )";
				tokens = tokenize(table_command);
				bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 3:{
						cout << "Enter: 'TEAM LOCATION SPONSOR'\n";
				vector<string> values;
				string temp;
				for(int i=0; i<3; i++){
					cin	>> temp;
					values.push_back(temp);
				}
				table_command = "INSERT INTO players VALUES FROM ( " + values[0] + ", ";
				table_command += (values[1]) + ", " + (values[2]) + ", " + (values[3]) + " )";
				tokens = tokenize(table_command);
				bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 4:{
					cout << "Give the name of the player you want to delete: \n";
					string temp;
					cin >> temp;
					table_command = "DELETE FROM players WHERE (PLAYER = " + temp + " )";
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 5:{
					cout << "Give the name of the team you want to delete: \n";
					string temp;
					cin >> temp;
					table_command = "DELETE FROM players WHERE (PLAYER = " + temp + " )";
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 6:{
					cout << "Give the name of the sponsor you want to delete: \n";
					string temp;
					cin >> temp;
					table_command = "DELETE FROM players WHERE (PLAYER = " + temp + " )";
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 7:{
					cout << "Give the name of the player you're updating: \n";
					string who;
					cin >> who;
					cout << "Enter their new Team.\n";
					
					vector<string> values;
					string temp;
					for(int i=0; i<1; i++){
						cin	>> temp;
						values.push_back(temp);
					}
					table_command = "UPDATE players SET TEAM = " + values[0] + " WHERE ( PLAYER = " + who + " )";					
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 8:{
				cout << "Give the name of the team you're updating: \n";
					string who;
					cin >> who;
					cout << "Enter their new Sponsor.\n";
					
					vector<string> values;
					string temp;
					for(int i=0; i<1; i++){
						cin	>> temp;
						values.push_back(temp);
					}
					table_command = "UPDATE teams SET SPONSOR = " + values[0] + " WHERE ( TEAM = " + who + " )";					
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 9:{
				cout << "Give the name of the Sponsor you're updating: \n";
					string who;
					cin >> who;
					cout << "Enter their new Product.\n";
					
					vector<string> values;
					string temp;
					for(int i=0; i<1; i++){
						cin	>> temp;
						values.push_back(temp);
					}
					table_command = "UPDATE sponsors SET PRODUCT = " + values[0] + " WHERE ( SPONSOR = " + who + " )";					
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 10:{
					table_command = "SHOW players";
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 11:{
					table_command = "SHOW teams";
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 12:{
					table_command = "SHOW sponsors";
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}
				case 13:{
					break;
					}
				case 14:{
					break;
					}
				case 15:{
					break;
					}
				case 16:{
					getline(cin, table_command);
					tokens = tokenize(table_command);
					bool success = Parse(tokens);
				if(success){
					cout << "Success!\n";
				}else{
				cout << "Fail\n";
				}
					break;
					}	
				case 17:{
					go_on = false;
					break;
					}
				default:
					cout << "That's not really on the menu...\n";
			
			
			}
		
		
		
		
		
		
		}
			
		table_command = "WRITE players";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		table_command = "WRITE teams";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		
		table_command = "WRITE sponsors";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		table_command = "CLOSE players";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		table_command = "CLOSE teams";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		
		table_command = "CLOSE sponsors";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		table_command = "EXIT";
		tokens = tokenize(table_command);
		Parse(tokens);
		
		
}