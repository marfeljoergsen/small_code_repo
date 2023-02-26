// g++ virt_destruct_deletion_verification.cpp &&./a.out
/*
/usr/bin/ld: /tmp/cc1fgN6s.o: in function `Child::~Child()':
virt_destruct_deletion_verification.cpp:(.text._ZN5ChildD2Ev[_ZN5ChildD5Ev]+0x22): undefined reference to `Base::~Base()'
collect2: error: ld returned 1 exit status

With a pure virtual function(), the compiler will throw an error at the time of
linking. As seen in the above output, the compiler tried to augment the call of
the base class destructor into the derived class destructor. But due to the
base class's destructor being unavailable, the linker exited with an error.
*/

// From: https://www.scaler.com/topics/virtual-destructor-in-cpp/

#include <iostream>
using namespace std;

class Base {
public:
  Base() { cout << "I'm base constructor..." << endl; }
  // pure virtual function = required to be implemented by a derived
  // class if the derived class is not abstract
  virtual ~Base() = 0; 
};

class Child : public Base {
public:
  //Child() { cout << "I'm child constructor..." << endl; }
  ~Child() {}
  //~Child(){ cout << "I'm a dying child..." << endl;} // don't delete child via base-pointer
};

int main() {
  Base *b = new Child;
  delete b;
  return 0;
}

