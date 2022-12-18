// g++ polymorphism_more_advanced.cpp && ./a.out                                                                                          1 ↵  14304  14:42:47
//20
//10

// dynamic allocation and polymorphism
#include <iostream>
using namespace std;

class Polygon {
  protected:
    int width, height;
  public:
    Polygon (int a, int b) : width(a), height(b) {}
    virtual int area (void) =0; // pure virtual (abstract)
    void printarea()
      { cout << this->area() << '\n'; }
};

class Rectangle: public Polygon {
  public:
    Rectangle(int a,int b) : Polygon(a,b) {}
    int area()
      { return width*height; } //{ return "Area: " << width*height; }
};

class Triangle: public Polygon {
  public:
    Triangle(int a,int b) : Polygon(a,b) {}
    int area()
      { return width*height/2; }
};

int main () {
  // dynamic memory allocation and constructor initializer
  Polygon * ppoly1 = new Rectangle (4,5);
  Polygon * ppoly2 = new Triangle (4,5);
  // polymorphism: Notice that the ppoly pointers:
  ppoly1->printarea(); // rectangle
  ppoly2->printarea(); // triangle
  // are declared being of type "pointer to Polygon", but the objects allocated
  // have been declared having the derived class type directly (Rectangle and
  // Triangle).
  delete ppoly1;
  delete ppoly2;
  return 0;
}

