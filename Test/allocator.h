#include "../TinySTL/allocator.h"
#include <iostream>
struct A{
    A(){
        std::cout << "A()" << std::endl;
    }
    ~A(){
        std::cout << "~A()" << std::endl;
    }

};


void test_1(){
    hstl::allocator<A> alloc;
    A *a = alloc.allocate();
    alloc.construct(a);
    alloc.destroy(a);
    alloc.deallocate(a);
    std::cout << "allocate test 1 passed" << std::endl;

}