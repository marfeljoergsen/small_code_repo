// g++ measure_time_SMALL2.cpp && ./a.out
// Elapsed time: 1.9e-05
//  * Be aware that clock() measures CPU time, not
//    actual time elapsed (which may be much greater).

#include <thread>
#include <ctime>
#include <iostream>

int main() {
  using namespace std;
  clock_t begin = clock();

  //code_to_time();
  std::this_thread::sleep_for(1500ms);

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Elapsed time: " << elapsed_secs << std::endl;
  std::cout << " * Be aware that clock() measures CPU time, not " << std::endl;
  std::cout << "   actual time elapsed (which may be much greater)." << std::endl;
}
