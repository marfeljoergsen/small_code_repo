// g++ initializer_list_3of7_reference_member.cpp && ./a.out
// 20
// 30

// *** From: https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/ ***

// Reference members must be initialized using Initializer List. In the
// following example, "t" is a reference member of Test class and is
// initialized using Initializer List.

// Initialization of reference data members
#include<iostream>
using namespace std;

class Test {
	int &t; // reference member of Test class
public:
	Test(int &t):t(t) {} //Initializer list must be used, always for reference members
	int getT() { return t; }
};

int main() {
	int x = 20;
	Test t1(x);
	cout<<t1.getT()<<endl;

	x = 30;
	cout<<t1.getT()<<endl;
	return 0;
}
/* OUTPUT:
	20
	30
*/

