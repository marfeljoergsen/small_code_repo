// g++ function_call_operator_PARENTHESIS_overloading.cpp && ./a.out
// First Distance : F: 11 I:10
// Second Distance :F: 30 I:120

// From: https://www.tutorialspoint.com/cplusplus/function_call_operator_overloading.htm

// The function call operator () can be overloaded for objects of class type.
// When you overload ( ), you are not creating a new way to call a function.
// Rather, you are creating an operator function that can be passed an
// arbitrary number of parameters.

#include <iostream>
using namespace std;
 
class Distance {
   private:
      int feet;             // 0 to infinite
      int inches;           // 0 to 12
      
   public:
      // required constructors
      Distance() {
         feet = 0;
         inches = 0;
      }
      Distance(int f, int i) {
         feet = f;
         inches = i;
      }
      
      // overload function call
      Distance operator()(int a, int b, int c) {
         Distance D;
         
         // just put random calculation
         D.feet = a + c + 10; // this works *EVEN* though these
         D.inches = b + c + 100 ; // variables are *PRIVATE* (because
         return D; // the overloading function is inside the class itself)
      }
      
      // method to display distance
      void displayDistance() {
         cout << "F: " << feet << " I:" << inches << endl;
      }   
};

int main() {
   Distance D1(11, 10), D2; // D1 = normal constructor and D2 = default (a
                            // default constructor is a constructor that either
                            // has no parameters, or if it has parameters, all
                            // the parameters have default values)
   cout << "First Distance : "; 
   D1.displayDistance(); // First Distance : F: 11 I:10
   //D1.feet = 123; // error: ‘int Distance::feet’ is private within this context

   D2 = D1(10, 10, 10); // invoke operator()
   cout << "Second Distance :"; 
   D2.displayDistance(); // Second Distance :F: 30 I:120

   return 0;
}

