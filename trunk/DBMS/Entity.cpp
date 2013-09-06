//This File Defines and implements the class Entity
//Created: Paul Gaughan, 9-4-13

#include <string>
#include <vector>

class Entity{
	private://Class data members go here.
	
	string type;
	int num_entries, num_fields;
	vector < vector < string> > Data;
	
	public://All external functions go here.
	
	//Creates an entity with no entries.  Argument is the top row of the table
	Entity(vector<string> fields){
		Data.push_back(fields);
		num_entries = 1;
		num_fields = fields.size();
		string type = Data[0][0];
	}
	
	void addEntry(vector<string> fields){
		Data.push_back(fields)
		num_entries++;
	}
	
	void addField(string field_name, string defaultVal = ""){
		Data[0].push_back(field_name);
		for(int i=1; i<Data.size(); i++){
			Data[i].push_back(defaultVal);
		}
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
	
}