// g++ polymorphism.cpp && ./a.out
// From: https://cplusplus.com/doc/tutorial/polymorphism/
// Rectangle area: 20
// Triangle area:  10

// pointers to base class
#include <iostream>
using namespace std;

class Polygon {
  protected:
    int width, height;
  public:
    void set_values (int a, int b)
      { width=a; height=b; }
};

/* When deriving a class from a base class, the base class may be inherited
 * through public, protected or private inheritance. The type of inheritance is
 * specified by the access-specifier as explained:

  1) Public Inheritance − When deriving a class from a public base class, public members of the base class become public members of the derived class and protected members of the base class become protected members of the derived class. A base class's private members are never accessible directly from a derived class, but can be accessed through calls to the public and protected members of the base class.

  2) Protected Inheritance − When deriving from a protected base class, public and protected members of the base class become protected members of the derived class.

  3) Private Inheritance − When deriving from a private base class, public and protected members of the base class become private members of the derived class.
*/

class Rectangle: public Polygon {
  public:
    int area()
      { return width*height; }
};

class Triangle: public Polygon {
  public:
    int area()
      { return width*height/2; }
};

int main () {
  Rectangle rect;
  Triangle trgl;
  Polygon * ppoly1 = &rect;
  Polygon * ppoly2 = &trgl;
  ppoly1->set_values (4,5); // rect: width*height
  ppoly2->set_values (4,5); // triangle: 1/2 w*h
  cout << "Rectangle area (w*h): " << rect.area() << '\n';
  cout << "Triangle area (1/2):  " << trgl.area() << '\n';
  return 0;
}

