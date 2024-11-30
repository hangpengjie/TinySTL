#ifndef TEST_AVL_TREE_H
#define TEST_AVL_TREE_H
#include "../TinySTL/avl_tree.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>


void test_1(){
    std::vector<int> v{1,2,3,4,5,6,7,8,9,10};
    hstl::avl_tree<int> tree;
    for(auto i: v){
        tree.insert_equal(i);
        assert(tree.size() == i);
        int j = 1;
        for(auto it = tree.begin(); it != tree.end(); ++it,++j){
            assert(*it == j);
        }

    }
    std::cout << "avl_tree test 1 passed" << std::endl;
}

void test_2(){
    std::random_device rd; // 随机数种子
    std::mt19937 gen(rd()); // 随机数引擎
    std::uniform_int_distribution<> dist(1, 1000); // 定义范围

    std::vector<int> v;
    for(int i = 0; i < 10000; ++i){
        v.push_back(dist(gen));
    }
    hstl::avl_tree<int> tree;
    for(auto i: v){
        tree.insert_equal(i);
    }
    assert(tree.size() == v.size());
    sort(v.begin(), v.end());
    int j = 0;
    for(auto it = tree.begin(); it != tree.end(); ++it,++j){
        assert(*it == v[j]);
    }
    std::cout << "avl_tree test 2 passed" << std::endl;
}

void test_3(){
    std::random_device rd; // 随机数种子
    std::mt19937 gen(rd()); // 随机数引擎
    std::uniform_int_distribution<> dist(1, 1000); // 定义范围

    std::vector<int> v;
    for(int i = 0; i < 10; ++i){
        v.push_back(dist(gen));
    }
    hstl::avl_tree<int> tree;
    for(auto i: v){
        tree.insert_equal(i);
        
    }
    assert(tree.is_balanced()); 
    assert(tree.size() == v.size());
    sort(v.begin(), v.end());
    int j = 0;
    for(auto it = tree.begin(); it != tree.end(); ++it,++j){
        assert(*it == v[j]);
    }
    std::cout << "avl_tree test 3 passed" << std::endl;
    
}


void test_4(){
    std::random_device rd; // 随机数种子
    std::mt19937 gen(rd()); // 随机数引擎
    std::uniform_int_distribution<> dist(1, 1000); // 定义范围

    std::vector<int> v;
    for(int i = 0; i < 9000; ++i){
        v.push_back(dist(gen));
    }
    hstl::avl_tree<int> tree;
    for(auto i: v){
        tree.insert_equal(i);
    }
    assert(tree.size() == v.size());
    std::shuffle(v.begin(), v.end(), gen);

    for(auto i: v){
        tree.remove_value(i);
        assert(tree.is_balanced());
    }
    
    
    std::cout << "avl_tree test 4 passed" << std::endl;
}

void test_5(){
    std::vector<int> v = {68,879,144,912,106,170,9,866,106};
    hstl::avl_tree<int> tree;
    for(auto i: v){
        tree.insert_equal(i);
    }
    int a;
    for(auto i: v){
        tree.remove_value(i);
        assert(tree.is_balanced());
    }
    std::cout << "avl_tree test 5 passed" << std::endl;
}
#endif