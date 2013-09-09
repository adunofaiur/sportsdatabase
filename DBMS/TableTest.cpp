//Test file for DBMS


#include "Table.cpp"
#include <iostream>
#include <vector>

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
	bool test1 = t.addEntry(Flds);
	if(test1) cout<<"Added row 1 successfully...\n";
	else cout<<"FAILED to add row 1\n";
	
	Flds[0] = "John Smith";
	Flds[1] = "010101010";
	Flds[2] = "CPSC";
	bool test2 = t.addEntry(Flds);
	if(test2) cout<<"Added row 2 successfully...\n\n";
	else cout<<"FAILED to add row 2\n\n";
	t.show();

	Flds[0]=("NAME");
	Flds[1]=("UIN");
	Flds[2]=("MAJOR");
	Table t2(Flds);
	Flds[0]=("Paul Gaughan");
	Flds[1]=("519001954");
	Flds[2]=("ECEN"); 
	t2.addEntry(Flds);
	Flds[0] = "John Smith";
	Flds[1] = "010101010";
	Flds[2] = "CPSC";
	t2.addEntry(Flds);
	cout<< "Created second table:\n\n";
	t2.show();
	
	Table t3(t);
	bool test3 = t3.setUnion(t2);
	if(	test3){ cout<<"\nTable 3: Set Union of first and second Tables:\n\n";	t3.show();}
	else cout<<"\n Set Union Failed.  Size mismatch.\n";
	
	Table t4(t);
	bool test4 = t4.setDifference(t2);
	if(	test4){ cout<<"\nTable 4: Set Difference of first and second Tables:\n\n";	t4.show();}
	else cout<<"\n Set Difference Failed.  Size mismatch.\n";
	
	Table t5(t);
	cout<<"\nTable 5: Computing Cartesian Product of first and second tables:\n\n";
	t5.crossProduct(t2);
	t5.show();
	
	cout<<"\n\nAttempting to compute set Union and Difference of first and fifth Tables\n\n";
	bool test6 = t5.setUnion(t);
	bool test7 = t5.setDifference(t);
	cout<<"Result:  Union Allowed?: " << test6<<"  Diff Allowed?: "<<test7<<endl;
	
	cout<<"\nTable 6: Projection of Names column from Table 3\n\n";
	vector<int> columns;
	columns.push_back(0);
	Table t6 = *(t3.projection(columns));
	t6.show();
	
	cout<<"\nTable 7: Selecting Row with Name = Paul Gaughan from Table 3\n\n";
	Table t7 = *(t3.rowQuerry("Paul Gaughan", "NAME"));
	t7.show();
	
	cout<<"\n\n\n\n";
	if(test1 && test2 && test3 && test4 && (!test6) &&(!test7)){
		cout<<"ALL TESTS PASSED!\n";
	}
	
}