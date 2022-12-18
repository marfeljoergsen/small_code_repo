// g++ cout_using_STL.cpp && ./a.out
// g++ -g -gdwarf-4 -DDEBUG -O0 -fno-omit-frame-pointer cout_using_STL.cpp && ./a.out

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main()
{
    std::vector<int>    userInput;

    std::cout << "Input some numbers (CTRL+D to end)\n";
    // Read until end of input.
    // Hit control D
    std::copy(std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>(),
              std::back_inserter(userInput)
             );

    // Print in Normal order
    std::cout << "\nPrinting in same order as entered:\n";
    std::copy(userInput.begin(),
              userInput.end(),
              std::ostream_iterator<int>(std::cout,",")
             );
    std::cout << "\n";

    // Update for C++11
    // Range based for is now a good alternative.
    std::cout << "C++11-method:\n";
    for(auto const& value: userInput)
    {
        std::cout << value << ",";
    }
    std::cout << "\n";

    // Print in reverse order:
    std::cout << "\nPrinting in REVERSED order (as entered):\n";
    std::copy(userInput.rbegin(),
              userInput.rend(),
              std::ostream_iterator<int>(std::cout,",")
             );
    std::cout << "\n";
}
