 //This File Defines and implements the class Table
//Created: Paul Gaughan, 9-4-13

#ifndef DBMS_TBL_CPP
#define DBMS_TBL_CPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Table{
	private://Class data members go here.
	
	string name;
	int num_entries, num_fields;
	vector < vector < string> > Data; 
	
	public://All external functions go here.
	
	class Malformed_table{};
	
	//Creates an entity with no entries.  Argument is the top row of the table
	Table(vector<string> fields){
		Data.push_back(fields);
		recalc();
	}
    Table(vector< vector< string > > D){//Constructor from data
		if(D.size()!=0){
			num_fields = D[0].size();
			for(int i=0; i< D.size(); i++){
				if(D[i].size() != num_fields){
					throw new Malformed_table;
				}
			}
		}
        Data = D;
		recalc();
    }	
	Table( const Table& t){	//Copy constructor
		for(int i=0; i< t.size(); i++){
			Data.push_back(t.getRow(i));
		}
		recalc();
	}

	Table(){}
	
	void setName(string newName){
		name = newName;
	}
	
	bool addEntry(vector<string> fields){
		if((fields.size() == num_fields) || num_entries ==0){
			Data.push_back(fields);
			recalc();
			return 1;
		}
		else return 0;
	}
	
	void addField(string field_name, string defaultVal = ""){
		Data[0].push_back(field_name);
		for(int i=1; i<Data.size(); i++){
			Data[i].push_back(defaultVal);
		}
	}
	string getName(){
		return name;
	}
	const int size() const {
		return num_entries;
	}
	bool rename(vector<string> fields){
		if(fields.size() != num_fields) return false;
		Data[0] = fields;
		recalc();
		return true;
	}
	
	bool renameField(string field_name, string new_name){
		for(int i = 0; i<num_fields; i++){
			if(Data[0][i].compare(field_name)){
				Data[0][i] = new_name;
				return true;
			}
		}
		return false;
	}
	const vector< vector< string> >* getTable() const{//For use by set operators
		return &Data;
	}
	const vector< string > getRow(int i) const{
		if(i>=0 && i < num_entries)	return Data[i];
		else throw new Malformed_table; 
	}
//Returns a Table of all rows that have Field_querried = Querry
	Table* rowQuerry(string Querry, string Field_querried = "", char op = '='){
		vector< vector < string > > Return_data;
		int comp = 0;
		if(op=='<') comp = -1;
		if(op=='>') comp=1;
		if(Field_querried.compare("")!=0){
			int column =-1;
			for(int i=0; i< num_fields; i++){
				if(Data[0][i].compare(Field_querried)==0){
					column = i;
					break;
				}
			}
			if(column == -1){
				return new Table(Return_data);
			}
			for(int i=1; i< num_entries; i++){
				if(Data[i][column].compare(Querry)==comp){
					Return_data.push_back(Data[i]);
				}
			}
		}else{
			for(int i =0; i<num_entries; i++){
				for(int j = 0; j<num_fields; j++){
					if(Data[i][j].compare(Querry)==comp){
						Return_data.push_back(Data[i]);
					}
				}
			}
		}
		return new Table(Return_data);
	}
	vector< string > removeRow(string key){
		vector<string> return_val;
		for(int i=0; i< num_entries; i++){
			if(Data[i][0].compare(key)==0){
				return_val = Data[i];
				Data.erase(Data.begin()+i);
				recalc();
				return return_val;
			}
		}
		return return_val;
	}

	vector< string > removeRow(vector <string> Row){
		vector<string> return_val;
		return_val.push_back("");
		for(int i=0; i< num_entries; i++){
			bool match = 1;
			for(int j=0; j<num_fields; j++){
				if(Data[i][j].compare(Row[j])!=0){
					match = 0;
					break;
				}
			}
			if(match){
				return_val = Data[i];
				Data.erase(Data.begin()+i);
				recalc();
				return return_val;

			}
		}
		return return_val;
	}
	
	Table* projection(vector< string > fields){
		vector<int> columns;
		for(int i =0; i<fields.size(); i++){
			for(int j=0; j<num_fields; i++){
				if (Data[0][j].compare(fields[i])){ //Found a column
					columns.push_back(j);
					break;
				}
			}
		}
		vector< vector< string > > return_table;
		if (columns.size()==0) return new Table(return_table);
		
		for( int i=0; i< num_entries; i++){
			vector< string > temp;
			for(int j=0; j<columns.size(); j++){
				temp.push_back(Data[i][columns[j]]);
			}
			return_table.push_back(temp);
		}
		return new Table(return_table);
	}
	Table* projection(vector< int > columns){
		vector< vector< string > > return_table;
		if (columns.size()==0) return new Table(return_table);
	
		for( int i=0; i< num_entries; i++){
			vector< string > temp;
			for(int j=0; j<columns.size(); j++){
				if(columns[j]>=0 && columns[j]<num_fields)
					temp.push_back(Data[i][columns[j]]);
			}
			return_table.push_back(temp);
		}
		return new Table(return_table);
	}
	
	bool setUnion(Table& t){//Stores union in this Table
		vector< vector <string> > temp;
		temp = *(t.getTable());
		if(temp.size()!= num_fields) return 0; //Sizes different, not set compatible
		for(int i = 0; i < temp.size(); i++){
		bool insert = true;
			for(int j=0; j < num_entries; j++){
				if(Data[j][0].compare(temp[i][0])==0){ //Entry is already in table, no dulications allowed.
					insert = false;
					break;
				}
			}
			if(insert){
				Data.push_back(temp[i]);
				num_entries++;
			}
		}
	return true;
	}
	
	bool setIntersection(Table& t){//Stores intersection in this Table
		vector< vector <string> > temp1;
		vector< vector <string> > temp2;
		temp1 = *(getTable());
		temp2 = *(t.getTable());
		if(temp2.size()!= num_fields) return 0; //Sizes different, not set compatible
		for(int i = 0; i < temp2.size(); i++){
		bool insert = false;
			for(int j=0; j < num_entries; j++){
				if(Data[j][0].compare(temp2[i][0])==0){ //Entry in both Tables, put in new Table
					insert = true;
					break;
				}
			}
			if(insert){
				temp1.push_back(temp2[i]);
				num_entries++;
			}
		}
		Data = temp1;
		recalc();
		return true;
	}
	
	
	bool setDifference(Table& t){//Stores difference in this Table
		vector< vector <string> > temp;
		temp = *(t.getTable());
		if(temp.size()!= num_fields) return 0; //Sizes different, not set compatible
		for(int i = 0; i < temp.size(); i++){
			for(int j=0; j < num_entries; j++){
				if(Data[j][0].compare(temp[i][0])==0){ //Entry is in both tables: remove entry
					Data.erase(Data.begin() + j); 
					num_entries--;
					break;
				}
			}
		}
	return true;
	}
	
	void show(){//Display function
		for(int i =0; i<num_entries; i++){
			for(int j = 0; j<num_fields; j++){
				cout<<"| "<<Data[i][j]<<" ";
			}
			cout<<"|\n";
		}
	}


	



	bool update(vector<string> Row, vector<string> fields, vector<string> new_values){
		int row = -1;
		for(int i=0; i< num_entries; i++){
			bool match = 1;
			for(int j=0; j<num_fields; j++){
				if(Data[i][j].compare(Row[j])!=0){
					match = 0;
					break;
				}
			}
			if(match == 1){
				row = i;
				break;
			}
		}
		if(row==-1) return false;
		for(int i=0; i<fields.size(); i++){
			for(int j=0; j< num_fields; j++){
				if(Data[0][j].compare(fields[i])==0){
					Data[row][j] = new_values[i];
					break;
				}
			}
		}
		return true;
	}
	bool update(string Key, string Field, string new_value){//Assign new value to the data entry with Key and Field
		int row = -1, column = -1;
		for(int i = 0; i< num_entries; i++){
			if(Data[i][0].compare(Key)){
				row = i;
				break;
			}
		}
		for(int i = 0; i< num_fields; i++){
			if(Data[0][i].compare(Key)){
				column = i;
				break;
			}
		}if(row>=0 && column >=0) Data[row][column] = new_value;
		else return false;
		return true;
	}
	
	void recalc(){//updates internal variables
		num_entries = Data.size();
		if(num_entries) num_fields = Data[0].size();
		else num_fields = 0;
		if(num_entries && num_fields) name = Data[0][0];
		else name = "";
	}
	
//Calculates the Cartesian Product of this table and another one and 
//stores teh result as the new value of this table	
	bool crossProduct(Table& t){
		vector< vector < string > > product;
		vector< string > temp1(Data[0]);
		vector< string > temp2 = t.getRow(0);
		temp1.insert(temp1.end(), temp2.begin(), temp2.end());
		product.push_back(temp1);
		for(int i=1; i<num_entries; i++){
			for(int j=1; j< t.size(); j++){
				temp1 = getRow(i);
				temp2 = t.getRow(j);
				temp1.insert(temp1.end(), temp2.begin(), temp2.end());
				product.push_back(temp1);
			}
		}
		Data = product;
		recalc();
	}
	bool read(){
		Data.clear();
		num_entries = 0; num_fields=0;
		string filename = name + ".db";
		vector<string> in;
		ifstream fin (filename.c_str(), ifstream::in);
		string s;
		while(fin.good()){
			fin>>s;
			if(s.compare("[()]")==0){
				addEntry(in);
				recalc();
				in.clear();
			}
			else{
				in.push_back(s);				
			}
		}

		fin.close();
		
		return true;
	}

	void save(){
		string filename = name + ".db";
		ofstream fout (filename.c_str() , ofstream::out);
		for(int i =0; i<num_entries; i++){
			for(int j = 0; j<num_fields; j++){
				fout<<Data[i][j]<<" ";
			}
			fout<<"[()] ";//Sequence that will not be likely to appear to indicate end of row
		}
		fout.close();
	}
};

//helper function
	Table initialize(){
		vector < string > in;
		string input = " ";
		int i = 0;
		Table t;
		ifstream fin("database.txt",ios::in); 
		while(fin.good()){

			getline(fin, input, '|'); 

			if(input == "[()]" && i == 0){
				Table q(in);
				for(int q = 0; q<in.size(); q++)
					cout << in[q] <<endl;

				in.clear();
				t = q;
				i++;
			}
			else if(input == "[()]"){ // [()] is the end of line delimeter
				t.addEntry(in);
				for(int q = 0; q<in.size(); q++)
					cout << in[q] <<endl;
				in.clear();
			}
			else
				in.push_back(input);
		}

		fin.close();
		return t;
	}

#endif