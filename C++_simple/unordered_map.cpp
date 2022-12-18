// g++ unordered_map.cpp && ./a.out
// Contribute 30
// Practice 20
// GeeksforGeeks 10


// C++ program to demonstrate functionality of unordered_map

#include <iostream>
#include <unordered_map>
using namespace std;

// unordered_map vs unordered_set : In unordered_set, we have only key, no
// value, these are mainly used to see presence/absence in a set. For example,
// consider the problem of counting the frequencies of individual words. We
// can’t use unordered_set (or set) as we can’t store counts. 

int main()
{
  // Declaring umap to be of <string, int> type
  // key will be of string type and mapped value will
  // be of int type
  unordered_map<string, int> umap;

  // inserting values by using [] operator
  umap["GeeksforGeeks"] = 10;
  umap["Practice"] = 20;
  umap["Contribute"] = 30;

  // Traversing an unordered map
  for (auto x : umap)
  cout << x.first << " " << x.second << endl;

}

