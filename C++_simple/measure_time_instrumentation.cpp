// g++ measure_time_instrumentation.cpp&&./a.out
// Run time of 15 ms sleep = 15ms
// Average runtime of 15 and 7 ms sleep = 11 ms

// From: https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
#include <iostream>
#include <chrono>
#include <functional>
#include <thread>

using namespace std::chrono_literals;

template <class TimeT  = std::chrono::milliseconds,
          class ClockT = std::chrono::steady_clock>
struct measure
{
    template <class F, class ...Args>
    static auto duration(F&& func, Args&&... args)
    {
        auto start = ClockT::now();
        std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
        return std::chrono::duration_cast<TimeT>(ClockT::now()-start);
    } 
};

void sleeper(std::chrono::milliseconds interval) {
    std::this_thread::sleep_for(interval);
}
 
int main() 
{
    std::cout << "Run time of 15 ms sleep = "
              << measure<>::duration(sleeper, 15ms).count() 
              << "ms\n";
    
    auto average = (
        measure<>::duration(sleeper, 15ms) + 
        measure<>::duration(sleeper, 7ms)
    ) / 2;
    std::cout << "Average runtime of 15 and 7 ms sleep = " 
              << average.count() << " ms\n";
    
	return 0;
}
