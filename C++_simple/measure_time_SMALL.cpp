// g++ -std=c++11 measure_time_SMALL.cpp && ./a.out
// Time difference = 0[µs]
// Time difference = 81[ns]

//***C++11 Style:***
#include <iostream>
#include <chrono>


int main() {
   std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
   std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

   std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
   std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
}
