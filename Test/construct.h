#include "../TinySTL/construct.h"
#include <cassert>
#include <iostream>
#include <new>

struct A{
    ~A() {
        std::cout << "A destructed" << std::endl;
    }
};

void test_1() {
    int* p = static_cast<int *>(std::malloc(sizeof(int)));
    hstl::construct(p);
    assert(*p == 0);
    hstl::construct(p, 10);
    assert(*p == 10);
    delete p;
    std::cout << "construct test 1 passed" << std::endl;
}

void test_2() {
    A* p = static_cast<A *>(std::malloc(sizeof(A)));
    hstl::construct(p);
    hstl::destroy(p);
    std::cout << "construct test 2 passed" << std::endl;
}