// g++ initializer_list_5of7_base_class_init.cpp && ./a.out
// A's Constructor called: Value of i: 10
// B's Constructor called%

// *** From: https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/ ***

/* Initializer list is needed for initialization of base class members : Like
 * point 3, the parameterized constructor of the base class can only be called
 * using Initializer List.
 */

#include <iostream>
using namespace std;

class A {
	int i;
public:
	A(int );
};

A::A(int arg) { // constructor for class A
	i = arg; // initialize member variable (int)
	cout << "A's Constructor called: Value of i: " << i << endl;
}

// Class B is derived from A
class B: A {
public:
	B(int );
};

B::B(int x):A(x) { //Initializer list must be used
   // the parameterized constructor of the base class "A" can only be called
   // using Initializer List.
	cout << "B's Constructor called";
}

int main() {
	B obj(10);
	return 0;
}

