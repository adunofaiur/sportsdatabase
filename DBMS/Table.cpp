 //This File Defines and implements the class Table
//Created: Paul Gaughan, 9-4-13

#ifndef DBMS_TBL_CPP
#define DBMS_TBL_CPP

#include <string>
#include <vector>
#include <iostream>

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
		num_entries = 1;
		num_fields = fields.size();
		string name = Data[0][0];
	}
	Table(vector< vector< string > > D){
		num_fields = D[0].size();
		for(int i=0; i< D.size(); i++){
			if(D[i].size() != num_fields){
				throw new Malformed_table;
			}
		}
		Data = D;
	}
	
	bool addEntry(vector<string> fields){
		if(fields.size() == num_fields){
			Data.push_back(fields);
			num_entries++;
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
	vector< vector< string > > rowQuerry(string Querry, string Field_querried = ""){
		vector< vector < string > > Return_data;
		if(Field_querried.compare("")!=0){
			int column =-1;
			for(int i=0; i< num_fields; i++){
				if(Data[0][i].compare(Field_querried)==0){
					column = i;
					break;
				}
			}
			if(column == -1){
				return Return_data;
			}
			for(int i=1; i< num_entries; i++){
				if(Data[i][column].compare(Querry)==0){
					Return_data.push_back(Data[i]);
				}
			}
		}else{
			for(int i =0; i<num_entries; i++){
				for(int j = 0; j<num_fields; j++){
					if(Data[i][j].compare(Querry)==0){
						Return_data.push_back(Data[i]);
					}
				}
			}
		}
		return Return_data;
	}
	vector< string > removeRow(string key){
		vector<string> return_val;
		for(int i=0; i< num_entries; i++){
			if(Data[i][0].compare(key)==0){
				return_val = Data[i];
				Data.erase(Data.begin()+i);
				return return_val;
			}
		}
		return return_val;
	}
	
	vector< vector < string > > projection(vector< string > fields){
		vector<int> columns;
		for(int i =0; i<fields.size(); i++){
			for(int j=0; j<num_fields; i++){
				if (Data[0][j].compare(fields[i])){ //Found a column
					columns.push_back(j);
					break;
				}
			}
			vector< vector< string > > return_table;
			if (columns.size()==0) return return_table;
			
			for( int i=0; i< num_entries; i++){
				vector< string > temp;
				for(int j=0; j<columns.size(); j++){
					temp.push_back(Data[i][j]);
				}
				return_table.push_back(temp);
			}
		}
	}
	
	bool setUnion(Table t){
		if(t.getName().compare(name) != 0) return 0; //Names different, not set compatible			
		vector< vector <string> > temp;
		temp = *(t.getTable());
		if(temp.size()!= num_fields) return 0; //Sizes different, not set compatible
		for(int i = 0; i < temp.size(); i++){
			bool insert = true;
			for(int j=0; j < num_entries; j++){
				if(Data[j][0].compare(temp[i][0])){ //Entry is already in table, no dulications allowed.
					insert = false; 
					break;
				}
				Data.push_back(temp[i]);
			}
		}
	}
	
	bool setDifference(Table t){
		if(t.getName().compare(name) != 0) return 0; //Names different, not set compatible			
		vector< vector <string> > temp;
		temp = *(t.getTable());
		if(temp.size()!= num_fields) return 0; //Sizes different, not set compatible
		for(int i = 0; i < temp.size(); i++){
			for(int j=0; j < num_entries; j++){
				if(Data[j][0].compare(temp[i][0])){ //Entry is in both tables: remove entry
					Data.erase(Data.begin() + j); 
					break;
				}
			}
		}
	}
	
	void show(){
		for(int i =0; i<num_entries; i++){
			for(int j = 0; j<num_fields; j++){
				cout<<"| "<<Data[i][j]<<" ";
			}
			cout<<"|\n";
		}
	}
	
	bool crossProduct(){//Calculates the Cartesian product of two sets.
	
	}
};

#endif