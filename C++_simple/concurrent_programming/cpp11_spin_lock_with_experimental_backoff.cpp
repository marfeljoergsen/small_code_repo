// g++ -g -std=c++2b cpp11_spin_lock_with_experimental_backoff.cpp && ./a.out
/*
counter value at the end: 20000
The counter value is correct! Yay!!
*/

#include "cpp11_spin_lock_with_experimental_backoff.hpp"
#include <iostream>
#include <thread>

SpinLock lock;

void increment_counter(int &counter, int operations) {
  for (int i = 0; i < operations; i++) {
    lock.lock();
    counter++;
    lock.unlock();
  }
}

int main() {
  const int numThreads = 100;
  const int opsPerThread = 200;

  int counter = 0;

  std::thread threads[numThreads];

  for (int i = 0; i < numThreads; i++) {
    threads[i] =
        std::thread(increment_counter, std::ref(counter), opsPerThread);
  }

  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
  }

  std::cout << "counter value at the end: " << counter << std::endl;

  if (counter != numThreads * opsPerThread) {
    std::cerr << "Error: Counter value is incorrect!!! Bummer..." << std::endl;
    return 1;
  }

  std::cout << "The counter value is correct! Yay!!" << std::endl;

  return 0;
}
