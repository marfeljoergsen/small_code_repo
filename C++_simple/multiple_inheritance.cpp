// g++ multiple_inheritance.cpp && ./a.out
// B's constructor called
// A's constructor called
// C's constructor called

#include<iostream>
using namespace std;

class A
{
   public:
      A() { cout << "A's constructor called" << endl; }
};

class B
{
   public:
      B() { cout << "B's constructor called" << endl; }
};

class C: public B, public A // Note the order: First B, then A.
{
   public:
      C() { cout << "C's constructor called" << endl; }
};

int main()
{
   C c;
   return 0;
}

