// g++ -std=c++20 cpp11_atomics.cpp && ./a.out
// counter value is: 3%

// From:
// https://medium.com/@joao_vaz/spin-lock-in-modern-c-with-atomics-memory-barriers-and-exponential-back-off-522798aca817

#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> counter{0};

void increment_counter() { counter++; }

int main() {
  std::thread t1(increment_counter);
  std::thread t2(increment_counter);
  std::thread t3(increment_counter);

  t1.join();
  t2.join();
  t3.join();

  // counter should be equal to 3
  std::cout << "counter value is: " << counter.load();
}

/*

=== The Spin Lock
A spin lock is a type of lock that uses busy-waiting to repeatedly check if a
lock is available. The basic idea is simple: when a thread wants to access a
shared resource, it checks if the lock is available. If the lock is not
available, the thread keeps spinning in a loop and checking the lock until it
becomes available.

While spin locks can be efficient in some scenarios, let’s repeat this again as
they can be wasteful and inefficient if the lock is held for very short periods
of time. In this case, the thread can waste a lot of CPU cycles spinning in a
loop and checking the lock, which can lead to poor performance and high power
consumption.


=== The Problem of Busy-Waiting
Busy-waiting is a common problem in concurrent programming, where a thread
repeatedly checks a condition in a loop and wastes CPU cycles while waiting for
the condition to become true. In the case of spin locks, busy-waiting occurs
when a thread repeatedly checks the lock in a loop and wastes CPU cycles while
waiting for the lock to become available.

Busy-waiting can lead to poor performance, high power consumption, and reduced
responsiveness. To address this issue, we can use a technique called
exponential backoff.


=== Exponential Backoff
Exponential backoff is a technique used in concurrent programming to reduce the
amount of time a thread spends waiting for a condition to become true. The
basic idea is to progressively increase the amount of time a thread waits after
each unsuccessful attempt, rather than repeatedly checking the condition in a
loop.

In the context of spin locks, exponential backoff can be used to reduce the
amount of time a thread spends spinning in a loop and checking the lock.
Instead of repeatedly checking the latter, the thread waits for an increasing
amount of time after each unsuccessful attempt, which can reduce power
consumption and improve performance.


=== Atomics
Atomics are a type of object in concurrent programming that allow multiple
threads to access and modify a variable in a thread-safe way. They provide the
necessary synchronization and memory barriers (see below for an explanation of
what they’re) to ensure that all threads see a consistent view of shared
memory.

In C++11 and later versions of the language, the std::atomic template is
provided to make it easy to use atomics. Here's an example of how to use an
atomic int variable:

*/
