//g++ riptutorial_lambda_capture.cpp && ./a.out
//The value of b=18
//The value of c=2

#include <iostream>
// https://riptutorial.com/cplusplus/example/1854/what-is-a-lambda-expression-

using namespace std;

int main() {
 int a = 2;                       // Define an integer variable
 //auto f = []()   { return a*9; }; // Error: 'a' cannot be accessed
 auto f = [a]()  { return a*9; }; // OK, 'a' is "captured" by value
 auto g = [&a]() { return a++; }; // OK, 'a' is "captured" by reference
                                  //      Note: It is the responsibility of the programmer
                                  //      to ensure that a is not destroyed before the
                                  //      lambda is called.
 auto b = f();                    // Call the lambda function. "a" is taken from the capture list and not passed here.
 auto c = g();                    // Call the lambda function. "a" is taken from the capture list and not passed here.
 
 cout << "The value of b=" << b << endl;
 cout << "The value of c=" << c << endl;
}

