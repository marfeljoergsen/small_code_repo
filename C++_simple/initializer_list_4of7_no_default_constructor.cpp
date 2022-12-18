//g++ initializer_list_4of7_no_default_constructor.cpp && ./a.out
// A's Constructor called: Value of i: 10
// B's Constructor called%

// Needed for initialization of member objects which do not have default
// constructor: In the following example, an object "a" of class "A" is data
// member of class "B", and "A" doesn’t have default constructor. Initializer
// List must be used to initialize "a".

// *** From: https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/ ***

#include <iostream>
using namespace std;

class A {
	int i;
public: // "A" doesn’t have default constructor
	A(int ); // no default value
};

A::A(int arg) { // A's constructor prints out a message when called
	i = arg;
	cout << "A's Constructor called: Value of i: " << i << endl;
}

// Class B contains object of A
class B {
	A a;
public:
	B(int ); // B take an int as argument, to it's constructor
};

B::B(int x):a(x) { // Initializer list must be used
	cout << "B's Constructor called";
}

int main() {
	B obj(10);
	return 0;
}
/* OUTPUT:
	A's Constructor called: Value of i: 10
	B's Constructor called
*/

