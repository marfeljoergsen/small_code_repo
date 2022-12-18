// g++ dynamic_memory_allocation.cpp && ./a.out
// Value of p: 29
// Value of r: 75.25
// Value store in block of memory: 1 2 3 4 5 %

// === From: https://www.geeksforgeeks.org/new-and-delete-operators-in-cpp-for-dynamic-memory/ ===

// C++ program to illustrate dynamic allocation
// and deallocation of memory using new and delete
#include <iostream>
#include <climits> // for ‘LLONG_MAX’
using namespace std;

int main ()
{
	// Pointer initialization to null
	int* p = NULL;

	// Request memory for the variable
	// using new operator
	p = new(nothrow) int;
	if (!p)
		cout << "allocation of memory failed\n";
      // If enough memory is not available in the heap to allocate, the new
      // request indicates failure by throwing an exception of type
      // std::bad_alloc, unless “nothrow” is used with the new operator, in
      // which case it returns a NULL pointer
	else
	{
		// Store value at allocated address
		*p = 29;
		cout << "Value of p: " << *p << endl;
	}

	// Request block of memory
	// using new operator
	float *r = new float(75.25);

	cout << "Value of r: " << *r << endl;

	// Request block of memory of size n
	//long long int n = LLONG_MAX; // see: "try_and_catch.cpp"
	int n = 8;
	int *q = new(nothrow) int[n];
	if (!q) {
		cout << "allocation of memory failed\n";
      return -1;
      // By default, when the new operator is used to attempt to allocate memory
      // and the handling function is unable to do so, a bad_alloc exception is
      // thrown. But when nothrow is used as argument for new, it returns a null
      // pointer instead.
   }
	else
	{
		for (int i = 0; i < n; i++)
			q[i] = i+1;

		cout << "Value store in block of memory: ";
		for (int i = 0; i < n; i++)
			cout << q[i] << " ";
	}

	// freed the allocated memory
	delete p; // Release memory pointed by pointer-variable
	delete r;

	// freed the block of allocated memory
	delete[] q;

	return 0;
}

