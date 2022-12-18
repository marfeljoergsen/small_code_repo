/*
 * This code won’t compile. Unlike nested blocks, where any identifier defined
 * in an outer block is accessible in the scope of the nested block, lambdas
 * can only access specific kinds of identifiers: global identifiers, entities
 * that are known at compile time, and entities with static storage duration.
 * search fulfills none of these requirements, so the lambda can’t see it.
 * That’s what the capture clause is there for.
 *
 * $ g++ learncpp_lambda_capture2_user_picks_substring.cpp
learncpp_lambda_capture2_user_picks_substring.cpp: In lambda function:
learncpp_lambda_capture2_user_picks_substring.cpp:40:22: error: ‘search’ is not captured
   40 |     return (str.find(search) != std::string_view::npos); // Error: search not accessible in this scope
      |                      ^~~~~~
learncpp_lambda_capture2_user_picks_substring.cpp:38:53: note: the lambda has no capture-default
   38 |   auto found{ std::find_if(arr.begin(), arr.end(), [](std::string_view str) {
      |                                                     ^
learncpp_lambda_capture2_user_picks_substring.cpp:35:15: note: ‘std::string search’ declared here
   35 |   std::string search{};
      |               ^~~~~~
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <string>

int main()
{
  std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };

  // Ask the user what to search for.
  std::cout << "search for: ";

  std::string search{};
  std::cin >> search;

  auto found{ std::find_if(arr.begin(), arr.end(), [](std::string_view str) {
    // Search for @search rather than "nut".
    return (str.find(search) != std::string_view::npos); // Error: search not accessible in this scope
  }) };

  if (found == arr.end())
  {
    std::cout << "Not found\n";
  }
  else
  {
    std::cout << "Found " << *found << '\n';
  }

  return 0;
}

