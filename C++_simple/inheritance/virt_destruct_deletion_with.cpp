// g++ virt_destruct_deletion_with.cpp && ./a.out
/*
Constructor of Base class is called
Constructor of Child class is called
Destructor of Child class is called
Destructor of Base class is called
*/

// From: https://www.scaler.com/topics/virtual-destructor-in-cpp/

#include <iostream>
using namespace std;

class Base {
public:
  Base() {
    cout << "Constructor of Base class is called" << endl;
  }
  //Defining virtual destructor
  virtual ~Base() {
    // At last it will be printed
    cout << "Destructor of Base class is called" << endl;
  }
};

//Inheriting features of Base class in Child class
class Child : public Base
{
public:
  Child() {
    cout << "Constructor of Child class is called" << endl;
  }
  //It will be called before calling the Base class destructor
  ~Child() {
    cout << "Destructor of Child class is called" << endl;
  }
};

int main() {
  Base *b = new Child; //  object refers to the Base class.
  delete b;            // Deleting the pointer object.
  return 0;
}
// In this program, a virtual destructor is used in the base class so that
// it calls the destructor of the child class before the base class's
// destructor is invoked. This releases the memory occupied and also resolves
// the problem of memory leaks.

