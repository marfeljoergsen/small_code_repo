// g++ initializer_list_7of7_better_performance_BEING_EFFICIENT.cpp && ./a.out
// 
//  A's constructor
//  A's Copy constructor
//  B's constructor%

/* The constructor of class B now uses initializer list to initialize its
 * member ‘a’. When Initializer list is used, the member ‘a’ of class B is
 * initialized directly from the parameter. So a call to A’s constructor is
 * reduced. In general, it is a good idea to use Initializer List to initialize
 * all members of a class, because it saves one extra assignment of members.
*/

#include<iostream>
using namespace std;

class A
{
  // data members of A
public:
  A()     { cout << "\n A's constructor"; /* Initialize data members */ }
  A(const A &a) { cout << "\n A's Copy constructor"; /* Copy data members */ }
  A& operator= (const A &a) // Assignemt Operator
  {
    // Handle self-assignment:
    if(this == &a) return *this;

    // Copy data members
    cout << "\n A's Assignment Operator"; return *this;
  }
};

class B
{
  A a;
  // Other members of B
public:
  B(A &a):a(a) { cout << "\n B's constructor"; }
};

int main()
{
  A a;
  B b(a);
  return 0;
}
