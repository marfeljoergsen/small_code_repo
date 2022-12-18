// g++ std_pair_example.cpp && ./a.out
// (1) Value-initialized: (0, 0)
// (2) Initialized with two values: (42, 3.1415)
// (4) Implicitly converted: (*, 3)
// (8) Piecewise constructed: ((0.123,7.7), aaaaaaaaaa)

#include <utility>
#include <string>
#include <complex>
#include <tuple>
#include <iostream>
 
int main()
{
    auto print = [](auto rem, auto const& pair) {
        std::cout << rem << "(" << pair.first << ", " << pair.second << ")\n";
    };
 
    std::pair<int, float> p1;
    print("(1) Value-initialized: ", p1);           // (0,0)
 
    std::pair<int, double> p2{42, 3.1415};
    print("(2) Initialized with two values: ", p2); // (42, 3.1415)
 
    std::pair<char, int> p4{p2};
    print("(4) Implicitly converted: ", p4);        // (*,3)
 
    std::pair<std::complex<double>, std::string> p6{
                    std::piecewise_construct,
                    std::forward_as_tuple(0.123, 7.7),
                    std::forward_as_tuple(10, 'a')};
    print("(8) Piecewise constructed: ", p6);       // ((0.123,7.7), aaaaaaaaaa)
}

