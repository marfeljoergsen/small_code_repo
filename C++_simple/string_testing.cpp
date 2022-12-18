// g++ string_testing.cpp && ./a.out
// g++ -g -gdwarf-4 -DDEBUG -O0 -fno-omit-frame-pointer string_testing.cpp && ./a.out

#include <iostream>
#include <string>
using namespace std;

int main()
{
   string firstName = "John ";
   string lastName = "Doe";
   string fullName = firstName + lastName;
   cout << fullName; 
}
