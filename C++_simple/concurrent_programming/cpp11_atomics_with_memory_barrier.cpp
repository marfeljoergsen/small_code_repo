// g++ -std=c++20 cpp11_atomics_with_memory_barrier.cpp && ./a.out
//Value we were expecting is correct: 5

// From: https://medium.com/@joao_vaz/spin-lock-in-modern-c-with-atomics-memory-barriers-and-exponential-back-off-522798aca817

#include <atomic>
#include <thread>
#include <iostream>

/* In this example, we use the fetch_add() method with
 * std::memory_order_relaxed to increment the counter variable. The
 * std::memory_order_relaxed option indicates that no memory barriers are
 * required, as we don't need to ensure any specific ordering or
 * synchronization between threads.
 */

std::atomic<int> counter(0);

void increment_counter() {
    counter.fetch_add(1, std::memory_order_relaxed);
}

int main() {
    std::thread t1(increment_counter);
    std::thread t2(increment_counter);
    std::thread t3(increment_counter);
    std::thread t4(increment_counter);
    std::thread t5(increment_counter);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();


    // counter should be equal to 5
    int expected = 5;
    int got = counter.load(std::memory_order_relaxed);
    if (got != expected) {
        std::cout << "Value not correct! We were expecting: " << expected << " but got: " << got << '\n';
    }

    std::cout << "Value we were expecting is correct: " << got << '\n';
}

/*

A memory barrier (also known as a memory fence) is a type of hardware or
software synchronization mechanism that ensures that all memory accesses on a
particular processor are executed in a specific order. They are used to enforce
sequential consistency, which is a consistency model in concurrent programming
that ensures that the order of operations in a program is the same for all
threads.

Memory barriers are used to prevent the reordering of memory access
instructions by a processor or compiler, which can lead to inconsistent
behavior in concurrent programs. For example, if two threads are accessing a
shared variable, a memory barrier can ensure that the order of the read and
write operations is consistent across all threads.

There are several types of memory barriers, which provide different levels of
synchronization and ordering guarantees. The most common types of memory
barriers are:

 1. Compiler Barriers: Compiler barriers are instructions that are inserted by
 a compiler to prevent the reordering of code. They ensure that the code is
 executed in the order in which it is written, in order to prevent incorrect
 behavior. They’re typically used to optimize code and improve performance, but
 they can also be used to prevent concurrency issues in certain cases.

 2. Processor Barriers: Processor barriers are instructions that are issued by
 a processor to ensure that memory operations are executed in a specific order.
 They prevent the processor from executing instructions out of order, which
 could lead to incorrect behavior in a concurrent program. Processor barriers
 are typically implemented using special instructions or cache coherency
 protocols.

 3. Store Barrier: A store barrier (also known as a write barrier) is a memory
 barrier that ensures that all memory stores before the barrier are visible to
 other threads. Store barriers prevent a processor from reordering memory
 stores, which could result in a write operation being executed after a read
 operation in another thread.

 4. Load Barrier: A load barrier (also known as a read barrier) is a memory
 barrier that ensures that all memory loads after the barrier are visible to
 the current thread. Load barriers prevent a processor from reordering memory
 loads, which could result in a read operation being executed before a write
 operation in another thread.

 5. Full Barrier: A full barrier (also known as a total barrier or memory
 fence) is a memory barrier that ensures that all memory operations before the
 barrier are visible to other threads, and all memory operations after the
 barrier are visible to the current thread. Full barriers provide the strongest
 level of synchronization and ordering guarantees, and are typically used when
 maximum synchronization is required.

In C++11 and later, memory barriers are provided by the std::memory_order
enum, which is used with atomic operations to provide the necessary
synchronization and memory barriers. The std::memory_order enum provides
several options for memory ordering, including memory_order_relaxed,
memory_order_acquire, memory_order_release, memory_order_acq_rel, and
memory_order_seq_cst.

 */
