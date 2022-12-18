// g++ 2d_matrix_simple.cpp && ./a.out
#include <iostream>

int main()
{
  const int rows = 3;
  const int cols = 4;
  std::cout << "This is a 2D matrix test...\n";

  //int** mat2d = new int[rows][cols] // <-- WRONG, SEE BELOW!

  if (false){
    // ==============================================================
    // Method 1: A dynamic 2D array can basically be a 1D-array of
    // pointers to 1D-arrays. You can initialize it using a loop:
    // ==============================================================
    int **mat2d = new int*[rows]; // the initial 1D-array
    for(int i = 0; i < rows; ++i) {
      mat2d[i] = new int[cols]; // new 1D-arrays for every row...
    }

    /* This is a dynamic array of pointers, initializing each pointer
       to its own, independent dynamic array. The advantage of this
       approach is that it gives you the indexing syntax you're used
       to, so if you want to find the value of the matrix at position
       x,y, you say:

       int val = matrix[ x ][ y ];

       This works because matrix[x] returns a pointer to an array,
       which is then indexed with [y]. Breaking it down:

       int* row = matrix[ x ];
       int  val = row[ y ];

       But this method is both fat and slow:

       Each "row" in the matrix is a separately allocated dynamic
       array. Making a heap allocation is expensive both in time and
       space. The allocator takes time to make the allocation,
       sometimes running O(n) algorithms to do it. And the allocator
       "pads" each of your row arrays with extra bytes for bookkeeping
       and alignment. That extra space costs...well...extra space. The
       deallocator will also take extra time when you go to deallocate
       the matrix, painstakingly free-ing up each individual row
       allocation.

       There's another reason it's slow. These separate allocations
       tend to live in discontinuous parts of memory. One row may be at
       address 1,000, another at address 100,000—you get the idea. This means
       that when you're traversing the matrix, you're leaping through memory
       like a wild person. This tends to result in cache misses that vastly
       slow down your processing time.
    */

    // Insert some data (C++, incl. NumPy for Python stores an array
    // as row major: A[0][0] A[0][1] A[0][2] A[1][1] ...  A[2][3]...
    int count = 0;
    for(int r = 0; r < rows; ++r) {
      for(int c = 0; c < cols; ++c) {
	mat2d[r][c] = count++;
      }
    }

    // Printing out the 2D-matrix
    std::cout << "mat2d =" << std::endl;
    for(int r = 0; r < rows; ++r) {
      for(int c = 0; c < cols; ++c) {
	std::cout << mat2d[r][c] << " ; ";
      }
      std::cout << std::endl;
    }

    // Cleaning up:
    for(int i = 0; i < rows; ++i) {
      delete [] mat2d[i]; // deleting all columns for the i'th row (in the 1D-array)
    }
    delete [] mat2d; // deleting the remaining 1D-array
  }
  else {
    // ==============================================================
    // Method 2: A single allocation of a large contigous memory block
    // on the heap, where mat2d[i][j] = mat2d[r][c] is then rewritten
    // as mat2d[r*colsPerRow+c] (or mat2d[row*numCols+col] ), because
    // data is stored in row major format: A[0][0] A[0][1] A[0][2]
    // A[1][1] ...  A[2][3]...
    // ==============================================================
    int *mat2d = new int[rows*cols];

    int count = 0;
    for(int r = 0; r < rows; ++r) {
      for(int c = 0; c < cols; ++c) {
	mat2d[r*cols+c] = count++;
      }
    }

    // Printing out the 2D-matrix
    std::cout << "mat2d =" << std::endl;
    for(int r = 0; r < rows; ++r) {
      for(int c = 0; c < cols; ++c) {
	std::cout << mat2d[r*cols+c] << " ; ";
      }
      std::cout << std::endl;
    }

    // Cleaning up:
    delete [] mat2d; // deleting the 1D-array
  } // if-else

}
