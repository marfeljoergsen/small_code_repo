// g++ initializer_list_1of7_basic.cpp && ./a.out
// x = 10, y = 15%

// *** From: https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/ ***

/* Why initializer list here? const data members must be initialized using
 * Initializer List. In the following example, "t" is a const data member of
 * Test class and is initialized using Initializer List. Reason for
 * initializing the const data member in the initializer list is because no
 * memory is allocated separately for const data member, it is folded in the
 * symbol table due to which we need to initialize it in the initializer
 * list.
 * */ 

#include<iostream>
using namespace std;

class Test {
	const int t;
public:
	Test(int t):t(t) {} //Initializer list must be used, because "const int t"!
	int getT() { return t; }
};

int main() {
	Test t1(10);
	cout<<t1.getT();
	return 0;
}

/* OUTPUT:
10
*/


