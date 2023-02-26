// g++ virt_destruct_deletion_with_pure.cpp  && ./a.out
/*
Destructor of child class is called
Pure virtual destructor is called
*/

// From: https://www.scaler.com/topics/virtual-destructor-in-cpp/

#include <iostream>
using namespace std;

class Base {
public:
  virtual ~Base() = 0; //Initializing Pure Virtual Destructor
  //  If we don't implement the base class pure virtual constructor outside the
  //  class, then the code will through an error when the derived class destructor
  //  calls the base class destructor:
  //Base::~Base() { //Function body of Pure Virtual Destructor
  //  cout << "Pure virtual destructor is called"<<endl;
  //}
  //    virt_destruct_deletion_with_pure.cpp:18:3: error: extra qualification ‘Base::’ on member ‘Base’ [-fpermissive]
  //       18 |   Base::~Base() { //Function body of Pure Virtual Destructor
  //          |   ^~~~
  //    virt_destruct_deletion_with_pure.cpp:18:3: error: ‘Base::~Base()’ cannot be overloaded with ‘virtual Base::~Base()’
  //    virt_destruct_deletion_with_pure.cpp:14:11: note: previous declaration ‘virtual Base::~Base()’
  //       14 |   virtual ~Base() = 0; //Initializing Pure Virtual Destructor
};
Base::~Base() { //Function body of Pure Virtual Destructor
  cout << "Pure virtual destructor is called"<<endl;
}

class Child : public Base { //Inheriting features of Base class in Child class
public:
  ~Child() {
    cout << "Destructor of child class is called"<<endl;
  }
};

int main() {
  Base *b = new Child();
  delete b;
  return 0;
}


