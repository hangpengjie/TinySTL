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
    assert(v.size() == 0);
    assert(v.capacity() == 16);
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
    assert(v.capacity() == 16);
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

struct D{
    int x;
    int y;
    int z;
    D(int a, int b, int c) : x(a), y(b), z(c) {
        std::cout << "A(int, int, int)" << std::endl;
    }
};
void test_14(){
    hstl::vector<D> v;
    v.emplace_back(1,2,3);
    assert(v[0].x == 1);
    assert(v[0].y == 2);
    assert(v[0].z == 3);
    std::cout << "vector test 14 passed" << std::endl;  
}

void test_15(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    v.erase(v.begin());
    assert(v.size() == 10);
    for(int i = 0; i < 10; i++){
        assert(v[i] == i + 1);
    }
    std::cout << "vector test 15 passed" << std::endl;
}

void test_16(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    v.erase(v.begin(), v.begin() + 5);
    assert(v.size() == 6);
    for(int i = 0; i < 6; i++){
        assert(v[i] == i + 5);
    }
    std::cout << "vector test 16 passed" << std::endl;
}

void test_17(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    auto it = v.erase(v.begin() + 5);
    assert(*it == 6);
    assert(v.size() == 10);
    for(int i = 0; i < 5; i++){
        assert(v[i] == i);
    }
    for(int i = 5; i < 10; i++){
        assert(v[i] == i + 1);
    }
    std::cout << "vector test 17 passed" << std::endl;
}

void test_18(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    v.resize(5);
    assert(v.size() == 5);
    for(int i = 0; i < 5; i++){
        assert(v[i] == i);
    }
    std::cout << "vector test 18 passed" << std::endl;
}

void test_19(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    v.resize(15,99);
    assert(v.size() == 15);
    for(int i = 11; i < 15; i++){
        assert(v[i] == 99);
    }
    std::cout << "vector test 19 passed" << std::endl;
}

void test_20(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    auto p = v.insert(v.begin() + 5, 99);
    assert(v.size() == 12);
    assert(v[5] == 99);
    assert(v[6] == 5);
    assert(*p == 99);
    std::cout << "vector test 20 passed" << std::endl;
}

void test_21(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    auto p = v.insert(v.begin() + 5, 3, 99);
    assert(v.size() == 14);
    assert(v[5] == 99);
    assert(v[6] == 99);
    assert(v[7] == 99);
    assert(v[8] == 5);
    assert(*p == 99);
    std::cout << "vector test 21 passed" << std::endl;
}

void test_22(){
    hstl::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10};
    v.reserve(100);
    assert(v.capacity() >= 100);
    assert(v.size() == 11);
    for(int i = 0; i < 11; i++){
        assert(v[i] == i);
    }
    std::cout << "vector test 22 passed" << std::endl;
}