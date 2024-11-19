#ifndef TEST_DEQUE_H
#define TEST_DEQUE_H

#include "../TinySTL/deque.h"
#include <iostream>
#include <cassert>


void test_1(){
    hstl::deque<int> d(10, 5);
    assert(d.size() == 10);
    for(auto it = d.begin(); it != d.end(); ++it){
        assert(*it == 5);
    }
    std::cout << "deque test 1 passed" << std::endl;
}



#endif