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
	Table(vector<string> fields){//Constructor that simmply 
		Data.push_back(fields);
		num_entries = 1;
		num_fields = fields.size();
		string name = Data[0][0];
	}
    Table(vector< vector< string > > D){
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
	const int size() const {
		return num_entries;
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
		else throw new Malformed_table{}; 
	}
	Table* rowQuerry(string Querry, string Field_querried = ""){
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
				return new Table(Return_data);
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
		return new Table(Return_data);
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
	
	bool setUnion(Table& t){
		if(t.getName().compare(name) != 0) return 0; //Names different, not set compatible			
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
	
	bool setDifference(Table& t){
		if(t.getName().compare(name) != 0) return 0; //Names different, not set compatible			
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
	bool update(int row, int column, string new_value){
		if(row>=0 && row< num_entries && column >=0 && column < num_fields) Data[row][column] = new_value;
		else return false;
		return true;
	}
	bool update(string Key, string Field, string new_value){
		int row = -1, column = -1;
		for(int i = 0; i< num_entries; i++){
			if(Data[i][0].compare(Key)){
				row = i;
				break;
			}
		}
		for(int i = 0; i< num_columns; i++){
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
	}
	bool crossProduct(Table& t){//Calculates the Cartesian product of two Tables.
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
};

#endif