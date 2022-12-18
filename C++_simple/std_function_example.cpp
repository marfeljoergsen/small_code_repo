// From: https://programming.vip/docs/c-std-function-example.html
// Class template std::function is a general polymorphic function wrapper.

/* ===
$ g++ std_function_example.cpp && ./a.out
Store free function
-9

 storage lambda
42

 Store to std::bind Result of call
31337

 Calls stored in member functions
2001
1001

 Calls stored in data member accessors
num_: 2000

 Calls stored in member functions and objects
2002

 Calls stored to member functions and object pointers
2003

 Calls stored in function objects
18
*/

#include <functional>
#include <iostream>

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_ = 1;
};

void print_num(int i)
{
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};

int main()
{
    // The signature it supports goes inside the angle brackets:
    // std::function<void()> takes zero arguments and returns nothing.
    // std::function< double( int, int ) > takes two int arguments and returns
    // double.

    // Store free function
    std::cout << "Store free function\n";
    std::function<void(int)> f_display = print_num; // return nothing, but take 1 int as argument
    f_display(-9);

    // Storage lambda
    std::cout << "\n storage lambda\n";
    std::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();

    // Results stored in the std::bind call
    std::cout << "\n Store to std::bind Result of call\n";

    // std::bind is a Standard Function Objects that acts as a Functional
    // Adaptor i.e. it takes a function as input and returns a new function
    // Object as an output with with one or more of the arguments of passed
    // function bound or rearranged. std::bind takes a function as its first
    // parameter and then that function’s argument as its parameter:
    // Parameter passed in when bind
    std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();

    // Calls stored in member functions
    std::cout << "\n Calls stored in member functions\n";
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(2000);
    f_add_display(foo, 1);
    f_add_display(1000, 1); // A foo object is constructed by default?

    // Calls stored in data member accessors
    std::cout << "\n Calls stored in data member accessors\n";
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';

    // Calls stored in member functions and objects
    std::cout << "\n Calls stored in member functions and objects\n";
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    f_add_display2(2);

    // Calls stored to member functions and object pointers
    std::cout << "\n Calls stored to member functions and object pointers\n";
    std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    f_add_display3(3);

    // Calls stored in function objects
    std::cout << "\n Calls stored in function objects\n";
    std::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);

    return 0;
}

