// g++ shared_ptr.cpp && ./a.out
/* (output addresses:)
pd.get()         = 0x55ec201eeec0
pd2.get()        = 0x55ec201eeee0
seq.back().get() = 0x55ec201ef350
*/

#include <memory> 
#include <iostream> 
#include <vector>

struct base { virtual ~base() {} virtual std::shared_ptr<base> clone() const = 0 ; } ;

struct derived_abstract : base {};

struct more_derived : derived_abstract
{ 
    virtual std::shared_ptr<base> clone() const 
    { 
        return std::make_shared<more_derived>( *this ) ; 
    }
};

int main() 
{ 
    std::shared_ptr<base> pb = std::make_shared<more_derived>() ; 
    std::shared_ptr<base> pb2 = pb->clone() ; 

    std::shared_ptr<derived_abstract> pd =
                          std::dynamic_pointer_cast<derived_abstract>(pb) ; 
    std::cout << "pd.get()         = " << pd.get() << '\n' ;

    std::shared_ptr<derived_abstract> pd2 = 
                          std::dynamic_pointer_cast<derived_abstract>(pb2) ; 
    std::cout << "pd2.get()        = " << pd2.get() << '\n' ;

    std::vector< std::shared_ptr<derived_abstract> > seq ;
    seq.push_back( std::dynamic_pointer_cast<derived_abstract>( pb->clone() ) ) ; 
    seq.push_back( std::dynamic_pointer_cast<derived_abstract>( seq[0]->clone() ) ) ; 
    std::cout << "seq.back().get() = " << seq.back().get() << '\n' ;
} 

