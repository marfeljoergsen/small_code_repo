// g++ 2d_matrix_C++11.cpp && ./a.out
// (you might need: "g++ -std=c++11" ?)
// =====================================
// Showing 'polluted memory':
// 2 4
// 3 5

// Re-allocating (deleting + allocating again):
// Showing that memory is not initialized:
// 0 4
// 3 5

// Showing the memory is zero-initialized:
// 0 0
// 0 0
// abi::__cxa_demangle(typeid(array).name(),0,0,&info) =
// double (*) [2]


#include <iostream>
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
using namespace std;

int main()
{
    const auto M = 2;
    const auto N = 2;

    // allocate (no initializatoin)
    auto array = new double[M][N];

    // pollute the memory
    array[0][0] = 2;
    array[1][0] = 3;
    array[0][1] = 4;
    array[1][1] = 5;

    cout << "Showing 'polluted memory':" << endl;
    for(int r = 0; r < M; r++)
    {
        for(int c = 0; c < N; c++)
            cout << array[r][c] << " ";
        cout << endl;
    }
    cout << endl;
    cout << "Re-allocating (deleting + allocating again):" << endl;
    // re-allocate, probably will fetch the same memory block (not portable)
    delete[] array;
    array = new double[M][N];

    cout << "Showing that memory is not initialized:" << endl;
    for(int r = 0; r < M; r++)
    {
        for(int c = 0; c < N; c++)
            cout << array[r][c] << " ";
        cout << endl;
    }
    cout << endl;

    delete[] array;

    // the proper way to zero-initialize the array
    array = new double[M][N]();

    cout << "Showing the memory is zero-initialized:" << endl;
    for(int r = 0; r < M; r++) {
        for(int c = 0; c < N; c++)
            cout << array[r][c] << " ";
        cout << endl;
    }

    int info;

    cout << endl;
    cout << "abi::__cxa_demangle(typeid(array).name(),0,0,&info) = " << endl;
    cout << abi::__cxa_demangle(typeid(array).name(),0,0,&info) << endl;

    return 0;
}
