// g++ virt_destruct_deletion_without.cpp&&./a.out
/*
 Constructor of Base class is called
 Constructor of Child class is called
 --
 Destructor of Base class is called
 --
*/

// From: https://www.scaler.com/topics/virtual-destructor-in-cpp/

#include <iostream>
using namespace std;

class Base {
public:
  Base() {
    cout << "Constructor of Base class is called"<<endl;
  }
  ~Base() {
    cout << "Destructor of Base class is called"<<endl;
  }
};

//Inheriting features of the Base class into the Child class
class Child : public Base {
public:
  Child() {
    cout << "Constructor of Child class is called"<<endl;
  }
  ~Child() {
    cout << "Destructor of Child class is called"<<endl;
  }
};

int main() {
  Child *ch = new Child(); // Initializing the object of: Base + Child class (in that order)
  Base *b = ch; // pointer to base
  std::cout <<"--" <<std::endl;
  delete b; // only base class destructor was called
  std::cout <<"--" <<std::endl;
  return 0;
}

/* In this program, we have used one parent class (Base) and a derived
 class (Child), inside which both constructors and destructors are defined in
 both classes.

 When we delete an object of a derived class using a pointer of the parent
 class, it shows an undefined behavior because it does not have a virtual
 destructor. So, when we delete the object of the Child class to release the
 space occupied, it invokes the destructor of the base class, but the
 destructor of the child class is not invoked. As seen in the output, the
 destructor of the Child class is not invoked because the base class pointer
 can only remove the destructor of the base class, which causes the problem of
 memory leak in the program.
 */

