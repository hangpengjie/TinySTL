#ifndef TEST_RB_TREE_H
#define TEST_RB_TREE_H
#include "../TinySTL/rb_tree.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>


void test_1(){
    std::random_device rd; // 随机数种子
    std::mt19937 gen(rd()); // 随机数引擎
    std::uniform_int_distribution<> dist(1, 1000); // 定义范围

    std::vector<int> v;
    for(int i = 0; i < 10000; ++i){
        v.push_back(dist(gen));
    }

    hstl::rb_tree<int, std::less<int>> rb_tree;
    for(int i = 0; i < 10000; ++i){
        rb_tree.insert_equal(v[i]);
    }


    std::cout << "rb_tree test 1 passed" << std::endl;
}

void test_2(){
    std::random_device rd; // 随机数种子
    std::mt19937 gen(rd()); // 随机数引擎
    std::uniform_int_distribution<> dist(1, 1000); // 定义范围

    std::vector<int> v;
    for(int i = 0; i < 10000; ++i){
        v.push_back(dist(gen));
    }

    hstl::rb_tree<int, std::greater<int>> rb_tree;
    for(int i = 0; i < 10000; ++i){
        rb_tree.insert_equal(v[i]);
    }
    std::sort(v.rbegin(), v.rend());
    auto it = rb_tree.begin();
    for(int i = 0; i < 10000; ++i,++it){
        assert(*it == v[i]);
    }

    std::cout << "rb_tree test 2 passed" << std::endl;
}
#endif