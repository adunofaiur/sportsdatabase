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
		num_entries = 0;
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
	

}