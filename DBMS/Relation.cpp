//This file defines and implements the Relation class
//Created: Paul Gaughan, 9-4-13

class Relation{
  private://Member data goes here
    
    string name;
	int num_entries;
    string relative1, relative2;//These will be entity/relation type names.  For instance, isSponsor would relate COMPANY and TEAM
    vector< vector < string> > Data;
    
  public://Functions go here
  
  Relation(string rel_name, string rel1, string rel2){
    name = rel_name;
	num_entries=1;
    relative1 = rel1;
    relative2 = rel2;
    Data.push_back(new vector<string>)
    Data[0].push_back(rel1);
    Data[0].push_back(rel2);
  }
  
  	vector< vector< string> > getTable(){//For use by set operators
		return Data;
	}
	vector< vector< string > > rowQuerry(string Field_querried = "", string Querry){
		vector< vector < string > > Return_data;
		if(Field_querried.compare("")!=0){
			int column =-1;
			if(relative1.compare(Field_querried)!=0){
				column=0;
			}
			if(relative2.compare(Field_querried)!=0){
				column=1;
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
				for(int j = 0; j<2; j++){
					if(Data[i][j].compare(Querry)==0){
						Return_data.push_back(Data[i]);
					}
				}
			}
		}
		return Return_data;
	}
	
}