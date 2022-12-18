// g++ try_and_catch.cpp && ./a.out
// bad_array_new_length caught: std::bad_array_new_length
// Program finished...

// bad_array_new_length example
#include <iostream>     // std::cout
#include <exception>    // std::exception
#include <new>          // std::bad_array_new_length

int main() {
  try {
    //int* p = new int[0];
    //int* p = new int[-1];
    int num = 5;
    int* p = new int[num-6];
  } catch (std::bad_array_new_length& e) {
    std::cerr << "bad_array_new_length caught: " << e.what() << '\n';
  } catch (std::exception& e) {   // older compilers may throw other exceptions:
    std::cerr << "some other standard exception caught: " << e.what() << '\n';
  }
  std::cout << "Program finished..." << std::endl;
}

