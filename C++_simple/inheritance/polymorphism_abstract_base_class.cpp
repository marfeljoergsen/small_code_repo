// g++ polymorphism_abstract_base_class.cpp && ./a.out
// Rectangle area: 20
// Triangle area: 10

// From: https://cplusplus.com/doc/tutorial/polymorphism/

// virtual members
#include <iostream>
using namespace std;

class Polygon {
  protected:
    int width, height;
  public:
    void set_values (int a, int b)
      { width=a; height=b; }

    /* A virtual member is a member function that can be redefined in a derived
       class, while preserving its calling properties through references. The
       syntax for a function to become virtual is to precede its declaration
       with the virtual keyword: */
    //virtual int area ()     // ***** VIRTUAL MEMBER FUNCTION "area" *****
    virtual int area () =0; // ***** VIRTUAL MEMBER FUNCTION "area" & ABSTRACT BASE CLASS *****
    /* The member function "area" has been declared as *PURE* virtual in the base class and
       it is later defined in each of the derived classes. */
    /* Classes that contain at least one pure virtual function are known as
     * abstract base classes. */
    /* The virtual function is pure virtual (abstract) because of the appending
     * = 0 after to the declaration. Classes with one or more pure
     * virtual functions are considered to be abstract, and cannot be
     * instantiated; only derived classes which define, or inherit definitions
     * for, all pure virtual functions can be instantiated. */
};

class Rectangle: public Polygon {
  public:
    int area ()
      { return width * height; }
};

class Triangle: public Polygon {
  public:
    int area ()
      { return (width * height / 2); }
};

int main () {
  Rectangle rect;
  Triangle trgl;
  //Polygon poly; // error: cannot declare variable ‘poly’ to be of abstract type ‘Polygon’
  Polygon * ppoly1 = &rect;
  Polygon * ppoly2 = &trgl;
  //Polygon * ppoly3 = &poly; // error: ‘poly’ was not declared in this scope; 
  ppoly1->set_values (4,5);
  ppoly2->set_values (4,5);
  //ppoly3->set_values (4,5); // error: ‘poly’ was not declared in this scope; 
  cout << "In this example, all three classes (Polygon, Rectangle and Triangle) have the\n";
  cout << "same members: width, height, and functions set_values and area.\n";
  cout << '\n';
  cout << "Rectangle area: " << ppoly1->area() << '\n'; // using virtual member func of derived class
  cout << "Triangle area: " << ppoly2->area() << '\n'; // using virtual member func of derived class
  //cout << "Polygon area: " << ppoly3->area() << '\n'; // using virtual member func of *BASE* class
  cout << '\n';
  cout << "The member function area has been declared as virtual in the base class\n";
  cout << "because it is later redefined in each of the derived classes. Non-virtual\n";
  cout << "members can also be redefined in derived classes, but non-virtual members of\n";
  cout << "derived classes cannot be accessed through a reference of the base class:\n";
  cout << "i.e., if virtual is removed from the declaration of area in the example\n";
  cout << "above, all three calls to area would return zero, because in all cases,\n";
  cout << "the version of the base class would have been called instead.\n";
  cout << "\n";;
  cout << "Therefore, essentially, what the virtual keyword does is to allow a\n";
  cout << "member of a derived class with the same name as one in the base class to be\n";
  cout << "appropriately called from a pointer, and more precisely when the type of the\n";
  cout << "pointer is a pointer to the base class that is pointing to an object of the\n";
  cout << "derived class, as in the above example.\n";
  cout << "\n";
  cout << "A class that declares or inherits a virtual function is called a polymorphic\n";
  cout << "class.\n";
  cout << "\n";
  cout << "Note that despite of the virtuality of one of its members, Polygon was a\n";
  cout << "regular class, of which even an object was instantiated (poly), with its own\n";
  cout << "definition of member area that always returns 0.\n";
  return 0;
}


