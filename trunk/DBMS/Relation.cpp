//This file defines and implements the Relation class
//Created: Paul Gaughan, 9-4-13

class Relation{
  private://Member data goes here
    
    string name;
    string relative1, relative2;//These will be entity/relation type names.  For instance, isSponsor would relate COMPANY and TEAM
    vector< vector < string> > Data;
    
  public://Functions go here
  
  Relation(string rel_name, string rel1, string rel2){
    name = rel_name;
    relative1 = rel1;
    relative2 = rel2;
    Data.push_back(new vector<string>)
    Data[0].push_back(relname);
    Data[0].push_back(rel1);
    Data[0].push_back(rel2);

  }
  
}