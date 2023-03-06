// $ g++ -std=c++20 template_meta_programming_in_cpp17_primer.cpp && ./a.out
/*
first of the list: 2
last of the list: 7
Front of the list: 2
Back of the list: 7
-----------------------
*/

// From: // https://medium.com/@joao_vaz/template-meta-programming-in-c-17-primer-1b493a22d51
// Formatted with "clang-format"

#include <iostream>
#include <tuple>
#include <utility>

template <typename... Args> struct type_list {};

template <typename T> constexpr bool is_even_v = (T::value % 2 == 0);

template <typename... Args> constexpr auto count_even(type_list<Args...>) {
  static_assert(sizeof...(Args) > 0);
  return (0 + ... + is_even_v<Args>);
}

template <typename... Args> constexpr auto sum(type_list<Args...>) {
  return (0 + ... + Args::value);
}

template <typename T, typename... Args>
constexpr auto front(type_list<T, Args...>) {
  return T{};
}

template <typename T, typename... Args>
constexpr auto back(type_list<T, Args...> tl) {
  if constexpr (sizeof...(Args) > 0) {
    return back(type_list<Args...>{});
  } else {
    return T{};
  }
}

template <typename... Args> auto unzip(type_list<Args...>) {
  return std::tuple<Args...>{};
}

int main() {
  using namespace std;

  // Define a type list of integers
  using my_list =
      type_list<std::integral_constant<int, 2>, std::integral_constant<int, 5>,
                std::integral_constant<int, 10>,
                std::integral_constant<int, 7>>;

  using my_list2 = type_list<>;

  // Use folding to count the number of even integers in the type list
  static_assert(count_even(my_list{}) == 2,
                "Incorrect count of even integers!");

  // Use folding to calculate the sum of the integers in the type list
  static_assert(sum(my_list{}) == 24, "Incorrect sum of integers!");

  // Use structure binding to extract the first, second, third, and last
  // elements of the type list
  auto [first, second, third, last] = unzip(my_list{});

  cout << "first of the list: " << first << endl;
  cout << "last of the list: " << last << endl;
  cout << "Front of the list: " << front(my_list{}) << endl;
  cout << "Back of the list: " << back(my_list{}) << endl;

  return 0;
}
