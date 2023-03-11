// From:
// https://medium.com/@joao_vaz/spin-lock-in-modern-c-with-atomics-memory-barriers-and-exponential-back-off-522798aca817

class SpinLock {
public:
  SpinLock() {}

  void lock() {
    retries = 0;
    // std::cout << "lock with retries=" << retries << std::endl;

    // test_and_set with memory_order_acquire provides a memory barrier
    // that ensures all previous writes are visible to this thread
    while (flag.test_and_set(std::memory_order_acquire)) {
      // spin until the lock is released
      backoff();
      retries++;
    }
  }

  void unlock() {
    // use clear with memory_order_release to release the lock
    // this uses a memory barrier that ensures all subsequent writes
    // are visible to other threads
    flag.clear(std::memory_order_release);
  }

private:
  void backoff() {
    const int max_retries = 8;

    // std::cout << "backoff with retries=" << retries << std::endl;
    if (retries < max_retries) {
      std::this_thread::yield();
    } else {
      // otherwise, use exponential backoff; calculate delay
      auto delay = std::chrono::microseconds(1 << (retries - max_retries));
      // sleep for the calculated delay
      std::this_thread::sleep_for(delay);
    }
  }

  std::atomic_flag flag = ATOMIC_FLAG_INIT;
  int retries{0};
};
