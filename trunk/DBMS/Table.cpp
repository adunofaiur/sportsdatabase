//This File Defines and implements the class Table
//Created: Paul Gaughan, 9-4-13

#include <string>
#include <vector>

class Table{
	private://Class data members go here.
	
	string name;
	int num_entries, num_fields;
	vector < vector < string> > Data; 
	
	public://All external functions go here.
	
	//Creates an entity with no entries.  Argument is the top row of the table
	Table(vector<string> fields){
		Data.push_back(fields);
		num_entries = 1;
		num_fields = fields.size();
		string name = Data[0][0];
	}
	
	bool addEntry(vector<string> fields){
		if(fields.size == num_fields){
			Data.push_back(fields)
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
	
	bool renameField(string field_name, string new_name){
		for(int i = 0; int i<num_fields; i++){
			if(Data[0][i].compare(field_name){
				Data[0][i] = new_name;
				return true;
			}
		}
		return false;
	}
	vector< vector< string> > getTable(){//For use by set operators
		return Data;
	}
	vector< vector< string > > rowQuerry(string Field_querried = "", string Querry){
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
	
	void show(){
		for(int i =0; i<num_entries; i++){
			for(int j = 0; j<num_fields; j++){
				cout<<"| "<<Data[i][j]<<" ";
			}
			cout<<"|\n";
		}
	}
}
