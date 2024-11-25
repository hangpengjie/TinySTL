#ifndef TEST_HEAP_H
#define TEST_HEAP_H
#include <iostream>
#include <type_traits>
#include <iterator>
#include <algorithm>

#include <cassert>
#include <vector>
#include "../TinySTL/heap.h"


void test_1() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    hstl::push_heap(v.begin(), v.end());
    std::cout << "heap test 1 passed" << std::endl;
}

void test_2() {
    std::vector<int> v = {};
    for(int i = 0; i < 10; ++i){
        v.push_back(i);
        hstl::push_heap(v.begin(), v.end());
        assert(v[0] == i);
    }
    std::cout << "heap test 2 passed" << std::endl;
}

void test_3() {
    std::vector<int> v = {};
    for(int i = 0; i < 10; ++i){
        v.push_back(i);
        hstl::push_heap(v.begin(), v.end());
        assert(v[0] == i);
    }
    for(int i = 9; i >= 0; --i){
        hstl::pop_heap(v.begin(), v.end());
        assert(v.back() == i);
        v.pop_back();
    }
    std::cout << "heap test 3 passed" << std::endl;
}

void test_4() {
    std::vector<int> v = {0,1, 2, 3, 4, 5, 6, 7, 8, 9};
    hstl::make_heap(v.begin(), v.end());
    for(int i = 9; i >= 0; --i){
        hstl::pop_heap(v.begin(), v.end());
        assert(v.back() == i);
        v.pop_back();
    }
    std::cout << "heap test 4 passed" << std::endl;
}

void test_5() {
    std::vector<int> v = {9,8,7,5,43,32,665,762,43,67,32,768,878};
    std::vector<int> v2 = v;
    hstl::make_heap(v.begin(), v.end());
    hstl::sort_heap(v.begin(), v.end());
    std::sort(v2.begin(), v2.end());
    for(int i = 0; i < v.size(); ++i){
        assert(v[i] == v2[i]);
    }
    std::cout << "heap test 5 passed" << std::endl;
    
}

#endif