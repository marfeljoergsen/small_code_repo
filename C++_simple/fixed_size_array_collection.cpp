// g++ fixed_size_array_collection.cpp &&  ./a.out                                                                           ✔  32748  13:30:17 
//Sizes of arrays are
//5
//5
//2
//
//Initial ar1 : 3 4 5 1 2 
//sorted ar1 : 1 2 3 4 5 
//Filled ar2 : 10 10 10 10 10 
//ar3 : a b %                                                                                                                                                                                                      

//--------------------------------------------
// From: https://www.geeksforgeeks.org/stdarray-in-cpp/
//--------------------------------------------
//This array container is defined for constant size arrays or (static size).
//The container wraps around fixed-size arrays and the information of its size
//are not lost when declared to a pointer. In order to utilize arrays, we need
//to include the array header:
//--------------------------------------------

// CPP program to demonstrate working of array
#include <array>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
using namespace std;

int main() {

   // construction uses aggregate initialization
   // double-braces required
   array<int, 5> ar1{{3, 4, 5, 1, 2}};
   array<int, 5> ar2 = {1, 2, 3, 4, 5};
   array<string, 2> ar3 = {{string("a"), "b"}};

   cout << "Sizes of arrays are" << endl;
   cout << ar1.size() << endl; // =5
   cout << ar2.size() << endl; // =5
   cout << ar3.size() << endl; // =2

   cout << "\nInitial ar1 : ";
   for (auto i : ar1)
      cout << i << ' '; // 3 4 5 1 2

   // container operations are supported
   sort(ar1.begin(), ar1.end());

   cout << "\nsorted ar1 : ";
   for (auto i : ar1)
      cout << i << ' '; // 1 2 3 4 5

   // Filling ar2 with 10
   ar2.fill(10);

   cout << "\nFilled ar2 : ";
   for (auto i : ar2)
      cout << i << ' '; // 10 10 10 10 10

   // ranged for loop is supported
   cout << "\nar3 : ";
   for (auto &s : ar3)
      cout << s << ' '; // a b %

   return 0;
}
