#ifndef TEST_DEQUE_H
#define TEST_DEQUE_H

#include "../TinySTL/deque.h"
#include <iostream>
#include <cassert>
#include <vector>


void test_1(){
    hstl::deque<int> d(10, 5);
    assert(d.size() == 10);
    for(auto it = d.begin(); it != d.end(); ++it){
        assert(*it == 5);
    }
    std::cout << "deque test 1 passed" << std::endl;
}


void test_2(){
    std::vector<int> v(10, 5);
    hstl::deque<int> d(v.begin(), v.end());
    assert(d.size() == 10);
    for(auto it = d.begin(); it != d.end(); ++it){
        assert(*it == 5);
    }
    std::cout << "deque test 2 passed" << std::endl;
}

void test_3(){
    hstl::deque<int> d(10);
    assert(d.size() == 10);
    for(auto it = d.begin(); it != d.end(); ++it){
        assert(*it == 0);
    }
    std::cout << "deque test 3 passed" << std::endl;
}

void test_4(){
    hstl::deque<int> d;
    assert(d.size() == 0);
    d.emplace_back(5);
    assert(d.size() == 1);
    
    std::cout << "deque test 4 passed" << std::endl;
}

void test_5(){
    hstl::deque<int> d;
    for(int i=0;i < 100000; ++i){
        d.emplace_back(i);
    }
    assert(d.size() == 100000);
    int i = 0;
    for(auto cur = d.begin(); cur != d.end(); ++ cur, ++i){
        assert(*cur == i);
    }
    std::cout << "deque test 5 passed" << std::endl;
}

void test_6(){
    hstl::deque<int> d;
    for(int i=0;i < 100000; ++i){
        d.emplace_front(i);
    }
    assert(d.size() == 100000);
    int i = 100000 - 1;
    for(auto cur = d.begin(); cur != d.end(); ++ cur, --i){
        assert(*cur == i);
    }
    std::cout <<"deque test 6 passed" << std::endl;
}

void test_7(){
    hstl::deque<int> d;
    for(int i=0;i < 100000; ++i){
        d.emplace_back(i);
    }
    assert(d.size() == 100000);
    int i = 0;
    for(auto cur = d.begin(); cur != d.end(); ++ cur, ++i){
        assert(*cur == i);
    }
    assert(!d.empty());
    for(i=0;i < 100000; ++i){
        d.pop_back();
    }
    assert(d.size() == 0);
    std::cout << "deque test 7 passed" << std::endl;
}

#endif