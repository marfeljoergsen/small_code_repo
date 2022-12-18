//g++ initializer_list_7of7_better_performance_NOT_EFFICIENT.cpp && ./a.out
//
// A's constructor
// A's constructor
// A's Assignment Operator
// B's constructor% 

/*
 * The first line of output is printed by the statement “A a1;” in main().
 The second line is printed when B’s member ‘a’ is initialized. This is
 important.  The third line is printed by the statement “this->a = a;” in B’s
 constructor.  The fourth line is printed by cout statement in B’s constructor.

 If we take a closer look at the above code, the constructor of class B is not
 efficient as member ‘a’ is first constructed with default constructor, and
 then the values from the parameter are copied using assignment operator. It
 may be a concern when class A is big, which generally is the case with many
 practical classes.
*/
 
#include<iostream>
using namespace std;
  
class A
{
    // data members of A
public:
    A ()           { cout << "\n A's constructor"; /* Initialize data members */ }
    A (const A &a) { cout << "\n A's Copy constructor";  /* copy data members */}
    A& operator= (const A &a) // Assignemt Operator
    {
        // Handle self-assignment:
        if(this == &a) return *this;
  
        // Copy data members
        cout << "\n A's Assignment Operator";  return *this;
    }
};
  
class B
{
    A a;
    // Other members of B
public:
    B(A &a) { this->a = a; cout << "\n B's constructor"; }
};
  
int main()
{
    A a1;
    B b(a1);
    return 0;
}
