//Test file for DBMS


#include "Table.cpp"
#include <iostream>

int main(){
	vector< string > Flds;
	Flds.push_back("NAME");
	Flds.push_back("UIN");
	Flds.push_back("MAJOR");
	cout<<" Table Created\n\n";
	Table t(Flds);
	t.show();
	
	cout<<"Adding two rows to the table\n\n";
	
	Flds[0] = "Jane Doe";
	Flds[1] = "101010101";
	Flds[2] = "CSCE";
	bool success = t.addEntry(Flds);
	if(success) cout<<"Added row 1 successfully...\n";
	else cout<<"FAILED to add row 1\n";
	
	Flds[0] = "John Smith";
	Flds[1] = "010101010";
	Flds[2] = "CPSC";
	success = t.addEntry(Flds);
	if(success) cout<<"Added row 2 successfully...\n\n";
	else cout<<"FAILED to add row 2\n\n";
	t.show();
}