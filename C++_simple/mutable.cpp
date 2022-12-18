// g++ mutable.cpp&& ./a.out

// --- Initial order details ---
// Customer name is: Pravasi Meet
// Food ordered by customer is: Ice Cream
// table no is: 3
// Total payable amount: 100
//
//  --- Changing placed order ---
// Customer name is: Pravasi Meet
// Food ordered by customer is: GulabJammuns
// table no is: 3
// Total payable amount: 150

#include <bits/stdc++.h>
#include <string.h>
using namespace std;

class Customer { // Customer Class
	// class Variables

   // The keyword mutable is mainly used to allow a particular data member of
   // const object to be modified. When we declare a function as const, the
   // this pointer passed to function becomes const. Adding mutable to a
   // variable allows a const pointer to change members.
	string name;
	mutable string placedorder;
	int tableno;
	mutable int bill;
   // mutable is particularly useful if most of the members should be constant but
   // a few need to be updatable. Data members declared as mutable can be modified
   // even though they are the part of object declared as const. You cannot use
   // the mutable specifier with names declared as static or const, or reference.

	// member methods
public:
	Customer(string s, string m, int a, int p) // constructor
	{
		name= s;
		placedorder=m;
		tableno = a;
		bill = p;
	}
   // *** The values of placedorder and bill data members are changed from const
   // *** function because they are declared as mutable. 
	void changePlacedOrder(string p) const // to change the place holder
	{
		placedorder=p;
	}
	void changeBill(int s) const { bill = s; } // change the bill

	void display() const // to display
	{
		cout << "Customer name is: " << name << endl;
		cout << "Food ordered by customer is: "
			<< placedorder << endl;
		cout << "table no is: " << tableno << endl;
		cout << "Total payable amount: " << bill << endl;
	}
};

int main() // Driver code
{
   cout << endl << " --- Initial order details ---" << endl;
	const Customer c1("Pravasi Meet", "Ice Cream", 3, 100);
	c1.display();

   cout << endl << " --- Changing placed order ---" << endl;
	c1.changePlacedOrder("GulabJammuns"); // was: "Ice Cream"
	c1.changeBill(150); // was: 100
	c1.display();
	return 0;
}

