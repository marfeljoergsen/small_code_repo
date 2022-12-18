WARNING: THIS DOES NOT COMPILE!

// *** From: https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/ ***

// It is better to initialize all class variables in Initializer List instead of
// assigning values inside body. Consider the following example:

// Without Initializer List
class MyClass {
	Type variable;
public:
	MyClass(Type a) { // Assume that Type is an already
					// declared class and it has appropriate
					// constructors and operators
	variable = a;
	}
};


// Here compiler follows following steps to create an object of type MyClass
// 
// 1. Type’s constructor is called first for “a”.
// 2. Default construct “variable”
// 3. The assignment operator of “Type” is called inside body of MyClass() constructor to assign
//     variable = a;
// 4. And then finally destructor of “Type” is called for “a” since it goes out of scope.
// 
// Now consider the same code with MyClass() constructor with Initializer List

// With Initializer List
class MyClass {
	Type variable;
public:
	MyClass(Type a):variable(a) { // Assume that Type is an already
					// declared class and it has appropriate
					// constructors and operators
	}
};

// With the Initializer List, the following steps are followed by compiler:
// 
// 1. Type’s constructor is called first for “a”.
// 2. Parameterized constructor of “Type” class is called to initialize:
//    variable(a). The arguments in the initializer list are used to copy
//    construct “variable” directly.
// 3. The destructor of “Type” is called for “a” since it goes out of scope.
// 
// As we can see from this example if we use assignment inside constructor body
// there are three function calls: constructor + destructor + one addition
// assignment operator call. And if we use Initializer List there are only two
// function calls: copy constructor + destructor call. See this post for a running
// example on this point.  This assignment penalty will be much more in “real”
// applications where there will be many such variables. Thanks to ptr for adding
// this point.



