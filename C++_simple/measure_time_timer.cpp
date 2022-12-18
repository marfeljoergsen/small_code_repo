// g++ measure_time_timer.cpp && ./a.out
// Code run for 15 ms
// Code run for 30465 us
// Resetting clock
// Code run for 5 ms

#include <iostream>
#include <chrono>
#include <thread>
#include <assert.h>

using namespace std::chrono_literals;

template <class DT = std::chrono::milliseconds,
          class ClockT = std::chrono::steady_clock>
class Timer
{
    using timep_t = decltype(ClockT::now());
    
    timep_t _start = ClockT::now();
    timep_t _end = {};

public:
    void tick() { 
        _end = timep_t{};
        _start = ClockT::now(); 
    }
    
    void tock() {
        _end = ClockT::now(); 
    }
    
    template <class duration_t = DT>
    auto duration() const { 
        // Use gsl_Expects if your project supports it.
        assert(_end != timep_t{} && "Timer must toc before reading the time"); 
        return std::chrono::duration_cast<duration_t>(_end - _start); 
    }
};

int main()
{
    Timer clock;  // Timer ticks upon construction.
    
    std::this_thread::sleep_for(15ms);
    clock.tock();
    std::cout << "Code run for " << clock.duration().count() << " ms" << std::endl;
    
    // No new .tic(), measure from the previous tic()
    std::this_thread::sleep_for(15ms);
    clock.tock();
    std::cout << "Code run for " << clock.duration<std::chrono::microseconds>().count() << " us" << std::endl;
    
    std::cout << "Resetting clock\n";
    clock.tick();
    std::this_thread::sleep_for(5ms);
    clock.tock();
    std::cout << "Code run for " << clock.duration().count() << " ms" << std::endl;
    
#if 0 // Example invalid use
    clock.tick();
    clock.duration();
#endif
}
