#ifndef TEST_LIST_H
#define TEST_LIST_H

#include "../TinySTL/list.h"
#include <algorithm>
#include <iostream>
#include <cassert>

void test_1(){
    hstl::list<int> l(10, 5);
    assert(l.size() == 10);

    for(auto it = l.begin(); it != l.end(); ++it){
        assert(*it == 5);
    }
    l.clear();
    assert(l.size() == 0);
    std::cout << "list test 1 passed" << std::endl;
}

void test_2(){
    hstl::list<int> l(10,9);
    assert(l.size() == 10);

    std::count(l.begin(), l.end(), 9);
    std::cout << "list test 2 passed" << std::endl;
}

void test_3(){
    hstl::list<int> l({1,2,3,4,5,6,7,8,9,10});
    assert(l.size() == 10);
    int i = 1;
    for(auto it = l.begin(); it != l.end(); ++it, ++i){
        assert(*it == i);
    }
    std::cout << "list test 3 passed" << std::endl;
}

void test_4(){
    hstl::list<int> l({1,2,3,4,5,6,7,8,9,10});
    hstl::list<int> l2 = l;
    assert(l2.size() == 10);
    int i = 1;
    for(auto it = l2.begin(); it != l2.end(); ++it, ++i){
        assert(*it == i);
    }
    std::cout << "list test 4 passed" << std::endl;
}

void test_5(){
    hstl::list<int> l({1,2,3,4,5,6,7,8,9,10});
    hstl::list<int> l2(l.begin(), l.end());
    assert(l2.size() == 10);
    int i = 1;
    for(auto it = l2.begin(); it != l2.end(); ++it, ++i){
        assert(*it == i);
    }
    std::cout << "list test 5 passed" << std::endl;
}

void test_6(){
    hstl::list<int> l({1,2,3,4,5,6,7,8,9,10});
    hstl::list<int> l2(5);
    assert(l2.size() == 5);
    l2 = l;
    assert(l2.size() == 10);
    int i = 1;
    for(auto it = l2.begin(); it != l2.end(); ++it, ++i){
        assert(*it == i);
    }
    std::cout << "list test 6 passed" << std::endl;
}


void test_7(){
    hstl::list<int> l({1,2,3,4,5,6,7,8,9,10});
    hstl::list<int> l2(15);
    assert(l2.size() == 15);
    l2 = l;
    assert(l2.size() == 10);
    int i = 1;
    for(auto it = l2.begin(); it != l2.end(); ++it, ++i){
        assert(*it == i);
    }
    std::cout << "list test 7 passed" << std::endl;
}
#endif