#include <iostream> // std::cout
#include <variant>
#include <string>
#include <cassert>
 
int main()
{
    std::cout << "Starting with variants v and w. They can have an int and a float value." << std::endl; 
    std::cout << "Their initial value is 0. v becomes 12 (line 1). std::get<int>(v) returns the value." << std::endl;
    std::variant<int, float> v, w;
    //if (false) {
    if (true) {
       std::cout << "    *** DEFAULT ***" << std::endl;
       v = 12; // v contains int
       //v = 12.345f; // v contains float
       //v = 12.345; // v contains double (does not work)
       int i = std::get<int>(v);
       std::cout << "i = " << i << std::endl;

       std::cout << "Below you see three possibilities to assign the variant v the variant w. But you" << std::endl;
       std::cout << "have to keep a few rules in mind: You can ask for the value of a" << std::endl;
       std::cout << "variant by type (get<int>) or by index (get<0>) - the type must be unique and the index valid:" << std::endl;
       w = std::get<int>(v);
       w = std::get<0>(v); // same effect as the previous line
       std::cout << "w = ";
    } else {
       int i = std::get<int>(v);
       //v = 12.345; // v contains double (does not work)
       v = 12.345f; // v contains float
    }
    visit( [](auto&& e) {
          std::cout << e << '\n';
          }, w );

    w = v; // same effect as the previous line
 
//  std::get<double>(v); // error: no double in [int, float]
//  std::get<3>(v);      // error: valid index values are 0 and 1
 
    try {
      //std::get<int>(w); // w contains int, not float: will throw
      std::get<float>(w); // w contains int, not float: will throw
    }
    catch (const std::bad_variant_access&) {
       std::cout << "CATCH EXCEPTION: the variant w holds an int value, this causes a std::bad_variant_access exception." << std::endl;
    }
 

    // === NEW EXAMPLE ===
    using namespace std::literals;

    std::cout << "If the constructor call or assignment call is unambiguous, a simple" << std::endl;
    std::cout << "conversion takes place. That is the reason that it's possible to construct" << std::endl;
    std::cout << "a std::variant<std::string> in line with a C-string below..." << std::endl; 
    std::variant<std::string> x("abc");
    // converting constructors work when unambiguous
    x = "def"; // converting assignment also works when unambiguous
 
    std::variant<std::string, void const*> y("abc");
    // casts to void const * when passed a char const *
    assert(std::holds_alternative<void const*>(y)); // succeeds
    std::cout << "... and the reason that it's possible to assign a new C-string to the variant:" << std::endl;
    y = "xyz"s;
    assert(std::holds_alternative<std::string>(y)); // succeeds
}

