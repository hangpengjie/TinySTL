#include "../TinySTL/vector.h"
#include <iostream>
#include <cassert>

struct A{
    int x;
    A():x(0){
        std::cout << "A()" << std::endl;
    }
    A(const A& other) : x(other.x) {
        std::cout << "A(const A&) "  << x << std::endl;
    }
};

struct B{
    
    B(){
        std::cout << "B()" << std::endl;
    }
};
void test_1(){
    hstl::vector<int> v;
    assert(v.size() == 16);
    std::cout << "vector test 1 passed" << std::endl;    
}

void test_2(){
    hstl::vector<int> v(10);
    assert(v.size() == 10);
    assert(v.capacity() == 16);
    std::cout << "vector test 2 passed" << std::endl;    
}

void test_3(){
    hstl::vector<A> v;
    assert(v.size() == 16);
    std::cout << "vector test 3 passed" << std::endl;    
}

void test_4(){
    hstl::vector<A> v(10);
    assert(v.size() == 10);
    assert(v.capacity() == 16);
    std::cout << "vector test 4 passed" << std::endl;    
}