/* g++ mutex.cpp && ./a.out
http://bar => fake content
http://foo => fake content
*/

// This example shows how a mutex can be used to protect an std::map shared
// between two threads.

#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string &url)
{
    // simulate a long page fetch
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string result = "fake content"; // =pair.second

    std::lock_guard<std::mutex> guard(g_pages_mutex);
    g_pages[url] = result; // key=pair.first ; g_pages=shared between both threads!
}

int main()
{
    std::thread t1(save_page, "http://foo");
    std::thread t2(save_page, "http://bar");
    t1.join();
    t2.join();

    // safe to access g_pages without lock now, as the threads are joined
    for (const auto &pair : g_pages) { // g_pages is shared
        std::cout << pair.first << " => " << pair.second << '\n';
    }
}

