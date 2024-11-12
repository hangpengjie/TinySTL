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

void test_5(){
    A a;
    a.x =1;
    hstl::vector<A> v(10,a);
    assert(v.size() == 10);
    assert(v.capacity() == 16);
    std::cout << "vector test 5 passed" << std::endl;    
}

void test_6(){
    hstl::vector<int> a(17);
    hstl::vector<int> b(a);
    assert(a.size() == b.size());
    assert(a.capacity() == b.capacity());
    assert(b.size() == 17);
    std::cout << "vector test 6 passed" << std::endl;
}

void test_7(){
    hstl::vector<int> a(17);
    hstl::vector<int> b(std::move(a));
    assert(b.size() == 17);
    assert(a.size() == 0);
    assert(a.capacity() == 0);
    
    std::cout << "vector test 7 passed" << std::endl;
}

void test_8(){
    hstl::vector<int> v({0,1,2,3,4,5,6,7,8,9,10});
    for(int i = 0; i < 11; i++){
        assert(v[i] == i);
    }
    std::cout << "vector test 8 passed" << std::endl;
}
void test_9(){
    hstl::vector<int> v({0,1,2,3,4,5,6,7,8,9,10});
    for(int i = 0; i < 11; i++){
        v[i] += 1;
        assert(v[i] == i + 1);
    }
    std::cout << "vector test 9 passed" << std::endl;
}

void test_10(){
    hstl::vector<int> v({0,1,2,3,4,5,6,7,8,9,10});
    v.~vector();
    std::cout << "vector test 10 passed" << std::endl;
}

struct B{
    ~B(){
        std::cout << "~B()" << std::endl;
    }
};
void test_11(){
    int n = 17;
    hstl::vector<B> v(n);
    std::cout << "Allocated " << n * sizeof(B) << " bytes" << std::endl;
    v.~vector();
    std::cout << "Deallocated " << n * sizeof(B) << " bytes" << std::endl;
    std::cout << "vector test 11 passed" << std::endl;
}

struct C{
    int x;
    C(int x):x(x){
        
    }
    C(const C& other){
        x = other.x;
        std::cout << "C(const C&)" << std::endl;
    }
};
void test_12(){
    hstl::vector<C> v = {C(0), C(1), C(2)};
    hstl::vector<C> v2 = std::move(v);
    assert(v2[0].x == 0);
    assert(v2[1].x == 1);
    assert(v2[2].x == 2);
    
    std::cout << "vector test 12 passed" << std::endl;
}

void test_13(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int a = 16;
    v.push_back(a);
    assert(v.capacity() == 24);
    assert(v.size() == 17);
    assert(v[16] == 16);
    std::cout << "vector test 13 passed" << std::endl;
}