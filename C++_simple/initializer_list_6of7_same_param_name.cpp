// g++ initializer_list_6of7_same_param_name.cpp && ./a.out
// 10%    

// *** From: https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/ ***

#include <iostream>
using namespace std;

class A {
	int i; // member name
public:
	A(int ); // Class "A" has no default constructor
	int getI() const { return i; }
};

// If constructor’s parameter name is same as data member name then the data
// member must be initialized either using this pointer or Initializer List. 
// Here, both member name and parameter name for A() is "i":
A::A(int i):i(i) { } // Either Initializer list or this pointer must be used
/* The above constructor can also be written as
A::A(int i) {
	this->i = i;
}
*/

int main() {
	A a(10);
	cout<<a.getI();
	return 0;
}
/* OUTPUT:
	10
*/
