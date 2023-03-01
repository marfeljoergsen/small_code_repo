//g++ concrete_vs_abstract_class.cpp&& ./a.out
/* Output:
Concrete Object Information
Object Creation Date : 23:26 PM 15 Dec 2013
*/

// * C++ Program to differentiate between concrete class and abstract class
#include <iostream>
#include <string>
using namespace std;
 
class Abstract {
    private:
        string info;
    public:
        virtual void printContent() = 0; 
};
 
class Concrete {
    private:
        string info;
    public:
        Concrete(string s) : info(s) { }
        void printContent() {
            cout << "Concrete Object Information\n" << info << endl;
        }
};
 
int main()
{
    /*
     * Abstract a;
     * Error : Abstract Instance Creation Failed
     */
    string s;
 
    s = "Object Creation Date : 23:26 PM 15 Dec 2013";
    Concrete c(s);
    c. printContent();
}

